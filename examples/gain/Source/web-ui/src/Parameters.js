import global from 'global'
import { useState, useEffect } from 'react';
var EventEmitter = require('eventemitter3');

class ParameterStore {

    constructor() {
        this.emitter = new EventEmitter();
    }

    get(id) {
        return global.cpp_getParameter && global.cpp_getParameter(id);
    }

    set(id, value) {
        return global.cpp_setParameterValue && global.cpp_setParameterValue(id, value);
    }

    subscribe(id, cb) {
        this.emitter.on(id, cb);
        return () => this.unsubscribe(id, cb);
    }

    unsubscribe(id, cb) {
        this.emitter.off(id, cb);
    }

    onCppParameterChange(id, value) {
        this.emitter.emit(id, value);
    }
}

const Parameters = global.Parameters = new ParameterStore();

export function useParameter(id) {

    const { min, value, max } = Parameters.get(id);
    const [latest, setValue] = useState(value);

    const onChange = v => setValue(() => v);

    useEffect(() => {
        return global.Parameters.subscribe(id, onChange, /* onError */);
    }, [id]);

    return {
      value: latest,
      min: min,
      max: max,
      setValue: (val) => global.Parameters.set(id, val)
    }
}
