
import global from 'global'

class AudioContext {

    createScriptProcessor() {

        return {
            connect: () =>{}
        }
    }
}

global.AudioContext = AudioContext;

//global.AudioContext = function AudioContext() {};