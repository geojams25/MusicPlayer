/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <memory>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MusicPlayerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener
                                                                           ,public juce::Slider::Listener 
                                                                           ,public juce::Timer 
{
public:
    MusicPlayerAudioProcessorEditor (MusicPlayerAudioProcessor&);
    ~MusicPlayerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::LookAndFeel_V3 lookV3;//used to change transport buttons to older style (more classy)
    //NOTE: declaring this before components that use it eliminates jassertation failures ;-)

    //buttons:
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton pauseButton;
    juce::TextButton stopButton;

    juce::Slider positionSlider;//follows transport pos and can be used to skip around
    juce::Slider volumeSlider;


    //MAKE SURE TO DECLARE ATTACHMENTS AFTER THEIR CONTROLS!
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volSliderAttachment;


    void openButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();
    void pauseButtonClicked();

    void buttonClicked (juce::Button* button) override;

    void sliderValueChanged(juce::Slider* slider) override;//essential function. music be included to inherit slider::listener
    void timerCallback() override;//essential function for Timer inherit.

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MusicPlayerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicPlayerAudioProcessorEditor)
};
