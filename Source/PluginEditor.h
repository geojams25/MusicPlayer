/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MusicPlayerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener
{
public:
    MusicPlayerAudioProcessorEditor (MusicPlayerAudioProcessor&);
    ~MusicPlayerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:


    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton pauseButton;
    juce::TextButton stopButton;


    


    void openButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();
    void pauseButtonClicked();

    void buttonClicked (juce::Button* button) override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MusicPlayerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicPlayerAudioProcessorEditor)
};
