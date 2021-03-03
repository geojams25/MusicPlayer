/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MusicPlayerAudioProcessorEditor::MusicPlayerAudioProcessorEditor (MusicPlayerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);



    openButton.setButtonText("Open File");
    addAndMakeVisible(&openButton);
    openButton.addListener(this);

    playButton.setButtonText("Play");
    addAndMakeVisible(&playButton);
    playButton.addListener(this);
    playButton.setEnabled(false);
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::seagreen);


    stopButton.setButtonText("Stop");
    addAndMakeVisible(&stopButton);
    stopButton.addListener(this);
    stopButton.setEnabled(false);
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::indianred);

    pauseButton.setButtonText("Pause");
    addAndMakeVisible(&pauseButton);
    pauseButton.addListener(this);
    pauseButton.setEnabled(false);


}

MusicPlayerAudioProcessorEditor::~MusicPlayerAudioProcessorEditor()

{
}

//==============================================================================
void MusicPlayerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::goldenrod);
    g.setFont (15.0f);
    g.drawFittedText ("Music Player", getLocalBounds(), juce::Justification::centredBottom, 1);
}

void MusicPlayerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    openButton.setBounds(10,10,getWidth()-20,30);
    playButton.setBounds(10,50,getWidth()-20,30);
    stopButton.setBounds(10,90,getWidth()-20,30);
    pauseButton.setBounds(10,130,getWidth()-20,30);
}

void MusicPlayerAudioProcessorEditor::openButtonClicked(){

    DBG("openButtonClicked()");//remove when working
    audioProcessor.chooseAudioFile();
    playButton.setEnabled(true);
    stopButton.setEnabled(false);
    pauseButton.setEnabled(false);
}

void MusicPlayerAudioProcessorEditor::playButtonClicked(){

    DBG("playButtonClicked()");
    stopButton.setEnabled(true);
    playButton.setEnabled(false);//greys out button
    pauseButton.setEnabled(true);
    audioProcessor.changeTransportState(audioProcessor.starting);//will start the transport
    //DBG(audioProcessor.starting);
}

void MusicPlayerAudioProcessorEditor::stopButtonClicked(){

    DBG("stopButtonClicked()");
    playButton.setEnabled(true);
    stopButton.setEnabled(false);
    pauseButton.setEnabled(false);

    if(audioProcessor.state == audioProcessor.playing)
        audioProcessor.changeTransportState(audioProcessor.stopping);
    else
        audioProcessor.changeTransportState(audioProcessor.stopped);//reset transport to 0.0

}


void MusicPlayerAudioProcessorEditor::pauseButtonClicked(){

    DBG("pauseButtonClicked()");
    playButton.setEnabled(true);
    stopButton.setEnabled(true);
    pauseButton.setEnabled(false);
    audioProcessor.changeTransportState(audioProcessor.pausing);

}


void MusicPlayerAudioProcessorEditor:: buttonClicked (juce::Button* button){

    if(button == &openButton){
        openButtonClicked();
    }

    else if(button == &playButton){
        playButtonClicked();
    }

    else if (button == &stopButton){
        stopButtonClicked();
    }

    else if(button == &pauseButton){

        pauseButtonClicked();
    }

}
