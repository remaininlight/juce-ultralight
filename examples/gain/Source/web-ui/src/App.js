import './App.css';

import global from 'global'
import './UltralightShim'
import { Dial } from 'react-nexusui';

function App() {

  return (
    <div className="App">
      <header className="App-header">
        <Dial interaction="radial"
              min={0}
              max={1} 
              onChange={global.setGain} />
        <p>
          Gain
        </p>
      </header>
    </div>
  );
}

export default App;
