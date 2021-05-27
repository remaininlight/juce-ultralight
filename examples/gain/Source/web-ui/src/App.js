import './App.css';

import global from 'global'
import './UltralightShim'
import { Dial } from 'react-nexusui';

import { useParameter } from './Parameters'

function ParameterDial({name: name}) {

  const param = useParameter(name);
  console.log('ParameterDial param ', param );
  console.log(param );
  console.log(param.value );

  let value = param.value;

  let onChange = function(v) {
    // Glitch (maybe caused by React memo in React Nexus) causes onChange to be called twice for each change
    if (value != v) {
      console.log('onChange');
      console.log(v);
      param.setValue(v);
      value = v;
    }
  }

  return (
      <div>
          <Dial interaction="radial"
                min={0} //{param.min}
                max={1} //{param.min} 
                value={0.4} //{param.value}
                onChange={onChange} />
      </div>
  );
}

/*
class ParameterDial extends React.Component {
  render() {
    return (
      <div>
          <Dial interaction="radial"
                min={0} //{param.min}
                max={1} //{param.min} 
                value={param.value}
                onChange={onChange} />
      </div>
    );
  }
}
*/


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

/*
        <Dial interaction="radial"
              min={0}
              max={1} 
            onChange={global.setGain} />
*/