import global from 'global'
import { useState, useEffect } from 'react';
var EventEmitter = require('eventemitter3');

class ParameterStore {

    constructor() {

        //this.params = {};
        this.emitter = new EventEmitter();
    }

    get(id) {
        /*
        return {
            min: 0,
            max: 1,
            value: 0.4
        }
        */
        return global.cpp_getParameter && global.cpp_getParameter(id);
    }

    set(id, value) {
        console.log('ParameterStore.set(', id, value, ')');
        return global.cpp_setParameterValue && global.cpp_setParameterValue(id, value);
    }

    subscribe(name, cb) {
        this.emitter.on(name, cb);
        return () => this.unsubscribe(name, cb);
    }

    unsubscribe(name, cb) {
        this.emitter.off('name', cb);
    }

    onCppParameterChange(name, value) {

        console.log('onCppParameterChange');
        console.log(name);
        console.log(value);
        this.emitter.emit(name, value);
    }
}

//global.handleCppParameterChange = handleCppParameterChange;
const Parameters = global.Parameters = new ParameterStore();

function getParameter(id) {
    return global.cpp_getParameter && global.cpp_getParameter(id);
}

/*
function setParameterValue(id, value) {
    console.log('setParameterValue', id, value);
    console.log(value);
    //console.log(global.cpp_setParameterValue);
    return global.cpp_setParameterValue && global.cpp_setParameterValue(id, value);
}
*/

function unsubscribeFromParameter(id, onChange) {

    /*
    global.cpp_unsubscribeFromParameter(
        id,
        callbackStore.add(onChange)
    )
    */
}

function subscribeToParameter(id, onChange, onError) {

    console.log('subscribeToParameter');
    /*
    global.cpp_subscribeToParameter 
        && global.cpp_subscribeToParameter(
            id,
            callbackStore.add(onChange)
        );
    */

    return () => unsubscribeFromParameter(id, onChange);
}

export function useParameter(id) {

    //let parameter = getParameter(id);
    const { min, value, max } = Parameters.get(id);
    const [latest, setValue] = useState(value);

    const onChange = v => {
                        setValue(() => v);
                     };

    useEffect(() => {

        return global.Parameters.subscribe(id, onChange);
        /*
        return global.ParsubscribeToParameter(
            id,
            onChange,
            e => {
                throw e;
            }
        );
        */
    }, [id]);

    return {
      value: latest,
      min: min,
      max: max,
      setValue: (val) => global.Parameters.set(id, val)
    }
}
