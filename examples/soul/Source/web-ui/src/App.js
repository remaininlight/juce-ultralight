import './App.css';

import global from 'global'
import './UltralightShim'
import { Dial } from 'react-nexusui';

import { useParameter } from './Parameters'

function ParameterDial({name: name}) {

  const param = useParameter(name);

  let value = param.value;

  let onChange = function(v) {
    // Glitch (maybe caused by React memo in React Nexus) causes onChange to be called twice for each change
    if (value != v) {
      param.setValue(v);
      value = v;
    }
  }

  return (
      <div>
          <Dial interaction="radial"
                min={param.min}
                max={param.max} 
                value={value} //{param.value}
                onChange={onChange} />
      </div>
  );
}

function App() {

  console.log('App()');
  return (
    <div className="App">
      <header className="App-header">
        <ParameterDial name="gain" />
        <p>
          Gain
        </p>
      </header>
    </div>
  );
}

export default App;