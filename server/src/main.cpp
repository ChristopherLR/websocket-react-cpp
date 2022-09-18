#include <iostream>
#include <signal.h>
#include <sys/signal.h>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/error.hpp>
#include <websocketpp/logger/levels.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;

server* wsServer;

void ws_on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "Received msg: " << msg->get_payload()
              << " from hdl: " << hdl.lock().get() << std::endl;

    if (msg->get_payload() == "stop-listening") {
        wsServer->stop_listening();
        return;
    }

    try {
        wsServer->send(hdl, msg->get_payload(), msg->get_opcode());
    } catch (websocketpp::exception& e) {
        std::cout << "Echo failed because: (" << e.what() << ")" << std::endl;
    }
};

void ws_on_http(websocketpp::connection_hdl hdl) {
    server::connection_ptr con = wsServer->get_con_from_hdl(hdl);

    con->set_status(websocketpp::http::status_code::ok);
    con->append_header("access-control-allow-origin", "*");
    con->append_header("content-type", "application/json; charset=UTF-8");
}

void ws_on_open(websocketpp::connection_hdl hdl) {
    wsServer->send(hdl, "Hello", 5, websocketpp::frame::opcode::text);
}

void sig_handler(int sig) {
    std::cout << "caught: " << sig << std::endl;
    std::cout << "Shutting down server" << std::endl;
    wsServer->stop_listening();
    exit(1);
};

int main() {
    wsServer = new server();

    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = sig_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    try {
        wsServer->set_access_channels(websocketpp::log::alevel::all);
        wsServer->clear_access_channels(
            websocketpp::log::alevel::frame_payload);
        wsServer->init_asio();
        wsServer->set_http_handler(&ws_on_http);
        wsServer->set_open_handler(&ws_on_open);
        wsServer->set_message_handler(&ws_on_message);

        const unsigned port = 9002;
        wsServer->listen(port);

        wsServer->start_accept();

        std::cout << "Running echo server on port: " << port << std::endl;
        wsServer->run();
    } catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown exception" << std::endl;
    }

    std::cout << "Exiting app" << std::endl;

    return 0;
}