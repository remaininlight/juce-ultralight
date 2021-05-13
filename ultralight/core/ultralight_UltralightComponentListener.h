#pragma once

#include <codecvt>
#include <optional>

#include "../../dependencies/ultralight-sdk/include/Ultralight/Ultralight.h"
//#include "../../dependencies/ultralight-sdk/include/AppCore/JSHelpers.h"
#include "../../dependencies/ultralight-sdk/include/Ultralight/CAPI.h"
#include "../../dependencies/ultralight-sdk/include/AppCore/AppCore.h"
//#include "../../dependencies/ultralight-sdk/include/JavaScriptCore/JavaScript.h"

namespace juce_ultralight {

    class UltralightComponentListener
    {
    public:
        /** Destructor. */
        virtual ~UltralightComponentListener() = default;

        /** Called when
         *
         * @param caller
         * @param frame_id
         * @param is_main_frame
         * @param url
         */
        virtual void windowObjectReady(ultralight::View *caller,
                                       uint64_t frame_id,
                                       bool is_main_frame,
                                       const ultralight::String &url)
        {
            juce::Logger::writeToLog("UltralightComponentListener::windowObjectReady");
        };
    };
}
