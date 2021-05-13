#pragma once

#include <map>


namespace juce_ultralight {

    /*
    class Parameters : public UltralightComponentListener
    {
    public:

        Parameters(UltralightComponent& webUI) : webUI(webUI)
        {
            juce::Logger::writeToLog("Parameters()");
            webUI.addListener(this);
        }

        ~Parameters() {}

        void windowObjectReady(ultralight::View *caller,
                               uint64_t frame_id,
                               bool is_main_frame,
                               const ultralight::String &url) override
        {

            juce::Logger::writeToLog("Parameters::windowObjectReady");
            webUI.registerNativeMethod ("cpp_getRawParameterValue", BindJSCallbackWithRetval(&Parameters::cpp_getRawParameterValue));
            webUI.registerNativeMethod ("cpp_subscribeToParameter", BindJSCallbackWithRetval(&Parameters::cpp_subscribeToParameter));
        };


        UltralightComponent& webUI;
    };
    */
}
