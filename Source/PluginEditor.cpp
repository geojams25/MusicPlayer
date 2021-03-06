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
    playButton.setLookAndFeel(&lookV3);


    stopButton.setButtonText("Stop");
    addAndMakeVisible(&stopButton);
    stopButton.addListener(this);
    stopButton.setEnabled(false);
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::indianred);
    stopButton.setLookAndFeel(&lookV3);

    pauseButton.setButtonText("Pause");
    addAndMakeVisible(&pauseButton);
    pauseButton.addListener(this);
    pauseButton.setEnabled(false);
    pauseButton.setLookAndFeel(&lookV3);

    positionSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    addAndMakeVisible(positionSlider);
    positionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false,50,30);
    positionSlider.addListener(this);
    positionSlider.setColour(juce::Slider::thumbColourId, juce::Colours::darkgoldenrod);
    positionSlider.setRange(0,10,1);//default. will be set properly when file loaded

    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    addAndMakeVisible(volumeSlider);
    volumeSlider.setColour(juce::Slider::trackColourId, juce::Colours::purple);
    volumeSlider.setRange(0.0,1.0);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox,true,0,0); 
    volumeSlider.addListener(this);
    volumeSlider.setSkewFactor(0.5);//arg <1 gives more of the slider over to lower values
    volumeSlider.setValue(0.25);
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
    g.drawFittedText ("Time", getLocalBounds(), juce::Justification::centredBottom, 1);
    g.setColour(juce::Colours::purple);
    g.drawText("Level",getWidth()/2-20,210,40,8,juce::Justification::centred);

}

void MusicPlayerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    openButton.setBounds(10,10,getWidth()-20,30);
    playButton.setBounds(10,50,getWidth()-20,30);
    stopButton.setBounds(10,130,getWidth()-20,30);
    pauseButton.setBounds(10,90,getWidth()-20,30);

    positionSlider.setBounds(10,getHeight()-70,getWidth()-20,50);
    volumeSlider.setBounds(50,getHeight()-120,getWidth()-100,20);
}

void MusicPlayerAudioProcessorEditor::openButtonClicked(){

    //DBG("openButtonClicked()");//remove when working
    juce::FileChooser chooser("Select File", juce::File::getSpecialLocation(juce::File::userMusicDirectory));
    bool fileChosen = chooser.browseForFileToOpen();

     if(fileChosen){

        audioProcessor.loadAudioFile(chooser.getResult());
        audioProcessor.transport.setPosition(0.0);

        playButton.setEnabled(true);
        stopButton.setEnabled(false);
        pauseButton.setEnabled(false);  
        
        positionSlider.setValue(0.0); //snap back to pos 0.0
        positionSlider.setRange(0.0, audioProcessor.transport.getLengthInSeconds(),1);//set slider range to match audio length
    }
}

void MusicPlayerAudioProcessorEditor::playButtonClicked(){

    //DBG("playButtonClicked()");
    stopButton.setEnabled(true);
    playButton.setEnabled(false);//greys out button
    pauseButton.setEnabled(true);
    audioProcessor.changeTransportState(audioProcessor.starting);//will start the transport
    startTimer(1000);//ms
}

void MusicPlayerAudioProcessorEditor::stopButtonClicked(){

    //DBG("stopButtonClicked()");
    playButton.setEnabled(true);
    stopButton.setEnabled(false);
    pauseButton.setEnabled(false);

    if(audioProcessor.state == audioProcessor.playing)
        audioProcessor.changeTransportState(audioProcessor.stopping);
    else
        audioProcessor.changeTransportState(audioProcessor.stopped);//reset transport to 0.0


    stopTimer();
    positionSlider.setValue(0.0);

}


void MusicPlayerAudioProcessorEditor::pauseButtonClicked(){

    //DBG("pauseButtonClicked()");
    playButton.setEnabled(true);
    stopButton.setEnabled(true);
    pauseButton.setEnabled(false);
    audioProcessor.changeTransportState(audioProcessor.pausing);


    stopTimer();

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


void MusicPlayerAudioProcessorEditor::sliderValueChanged(juce::Slider* slider){

    if(slider == &positionSlider){

        audioProcessor.transport.setPosition(slider->getValue());
    }

    else if(slider == &volumeSlider){

        //audioProcessor.transport.setGain(juce::Decibels::decibelsToGain(slider->getValue()));//for dB (change range to -80, 0 etc)
        audioProcessor.transport.setGain(slider->getValue());
    }

}


void MusicPlayerAudioProcessorEditor::timerCallback(){

    positionSlider.setValue(audioProcessor.transport.getCurrentPosition(),juce::dontSendNotification);//make slider update to audio pos (follow)

    // above line causes audible clicks on callback (every second)
}
