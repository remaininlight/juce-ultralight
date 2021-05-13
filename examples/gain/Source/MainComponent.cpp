#include "MainComponent.h"

//==============================================================================
/*
AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    AudioProcessorValueTreeState::ParameterLayout params (
        std::make_unique<AudioParameterFloat>(
            "Gain",
            "Gain",
            NormalisableRange<float>(0.0, 1.0),
            0.8,
            juce::String(),
            AudioProcessorParameter::genericParameter,
            [](float value, int maxLength) {
                return juce::String(Decibels::gainToDecibels(value), 1) + "dB";
            },
            nullptr
        )
    );

    return params;
}
*/

MainComponent::MainComponent() : webUI(true)
{
    
    webUI.addListener (this);
    addAndMakeVisible (webUI);
    webUI.loadURL ("http://localhost:3000");
    setSize (1200, 800);
}

MainComponent::~MainComponent() {}

//==============================================================================
void MainComponent::paint (juce::Graphics& g) {}

void MainComponent::resized()
{
    webUI.setBounds (getLocalBounds());
}

void MainComponent::windowObjectReady(ultralight::View* caller,
                                      uint64_t frame_id,
                                      bool is_main_frame,
                                      const ultralight::String& url)
{
    webUI.on ("setGain", BindJSCallbackWithRetval(&MainComponent::setGain));
}

JSValue MainComponent::setGain(const JSObject& thisObject, const JSArgs& args)
{

    juce::Logger::writeToLog("MainComponent::setGain");
    return JSValue("Hello from C++!");
}
