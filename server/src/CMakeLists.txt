add_executable(streaming main.cpp)

set(LOCAL_LIBS
)

foreach(lib IN LISTS LOCAL_LIBS)
    include_directories(${lib})
    add_subdirectory(${lib})
endforeach()


target_link_libraries(streaming PUBLIC ${LOCAL_LIBS})
target_link_libraries(streaming PRIVATE websocketpp::websocketpp)
