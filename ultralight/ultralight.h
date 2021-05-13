/*******************************************************************************
 * This file declares a JUCE module for the shared library code implemented in
 * this directory. The block below is read by the Projucer to automatically
 * generate project code that uses the module. For details, see the
 * JUCE Module Format.txt file in the JUCE directory.

   BEGIN_JUCE_MODULE_DECLARATION
    ID:                 ultralight
    vendor:             Mark Claydon
    version:            0.1.0
    name:               Ultralight
    description:        A web browser
    minimumCppStandard: 17
    dependencies:       juce_core, juce_graphics, juce_gui_basics
    searchpaths:   ../dependencies/json-rpc-cxx/vendor
   END_JUCE_MODULE_DECLARATION
*******************************************************************************/
//  OSXLibs:       librpc

#pragma once

#define ULTRALIGHT_H_INCLUDED

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

//#define MSGPACK_ENDIAN_LITTLE_BYTE 1
//#include "dependencies/rpclib/include/rpc/server.h"

#include "core/ultralight_SplitComponent.h"
#include "core/ultralight_Surface.h"
#include "core/ultralight_Browser.h"
#include "core/ultralight_UltralightComponentListener.h"
#include "core/ultralight_UltralightComponent.h"
#include "core/ultralight_Parameters.h"
