#pragma once

#include <JuceHeader.h>

class MainComponent  : public juce::Component, public juce_ultralight::UltralightComponentListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void windowObjectReady(ultralight::View* caller,
                           uint64_t frame_id,
                           bool is_main_frame,
                           const ultralight::String& url) override;

    JSValue setGain(const JSObject& thisObject, const JSArgs& args);

private:
    //==============================================================================

    juce_ultralight::UltralightComponent webUI;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
