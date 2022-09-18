import React, { useEffect } from 'react';
import useWebSocket, { ReadyState } from 'react-use-websocket';

import './App.css';

function App() {
  return (
    <div className="App">
      <header className="App-header">
        <TextInput/>
      </header>
    </div>
  );
}

function TextInput() {

  const { sendMessage, lastMessage, readyState } = useWebSocket("ws://localhost:9002");

  const changed = (e) => {
      const { target: { value } } = e;
      sendMessage(value);
      console.log(`Sent: ${value}`);
  };

  const connectionStatus = {
      [ReadyState.CONNECTING]: 'Connecting',
      [ReadyState.OPEN]: 'Open',
      [ReadyState.CLOSING]: 'Closing',
      [ReadyState.CLOSED]: 'Closed',
      [ReadyState.UNINSTANTIATED]: 'Uninstantiated',
    }[readyState];

  return (
      <React.Fragment>
          <input onChange={changed} />
          <div>{connectionStatus}</div>
          <div>{lastMessage ? lastMessage.data : null}</div>
      </React.Fragment>
  );
}

export default App;
