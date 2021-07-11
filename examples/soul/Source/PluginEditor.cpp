/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainAudioProcessorEditor::GainAudioProcessorEditor (GainAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), webUI(p.apvts)
{

    webUI.addListener (this);
    addAndMakeVisible(webUI);
    webUI.loadURL("http://localhost:3000/");

    setResizable (true, true);
    setResizeLimits (400, 240, 400 * 2, 240 * 2);
    getConstrainer ()->setFixedAspectRatio(400.0 / 240.0);
    setSize (800, 480);
}

GainAudioProcessorEditor::~GainAudioProcessorEditor()
{
}

//==============================================================================
void GainAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void GainAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    webUI.setBounds(area);
}

void GainAudioProcessorEditor::windowObjectReady(ultralight::View* caller,
                                                 uint64_t frame_id,
                                                 bool is_main_frame,
                                                 const ultralight::String& url)
{
    //webUI.registerNativeMethod ("setGain", BindJSCallbackWithRetval(&HarmonyEngineAudioProcessorEditor::setGain));
}