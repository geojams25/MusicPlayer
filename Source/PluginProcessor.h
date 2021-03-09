/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <memory>
//==============================================================================
/**
*/
class MusicPlayerAudioProcessor  : public juce::AudioProcessor,
                                   public juce::ChangeListener
{
public:
    //==============================================================================
    MusicPlayerAudioProcessor();
    ~MusicPlayerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    
    enum transportState{
        stopped,
        starting,
        playing,
        stopping,
        pausing,
        paused
    };// 0,1,2,3,4,5
    transportState state; //this state variable can be set to any of the "enums" above.


    void changeListenerCallback(juce::ChangeBroadcaster *source) override;//have to include this if we inherit from changeListener class
    void changeTransportState(transportState newState);
    void chooseAudioFile();
    void loadAudioFile(const juce::File& file);
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    juce::AudioTransportSource transport;
    juce::File currentlyLoadedFile;
    bool fileLoaded;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource_ptr; 
    juce::AudioFormatManager formatManager; //This class contains a list of audio formats (such as WAV, AIFF,
   // Ogg Vorbis, and so on) and can create suitable objects for reading audio data from these formats.

    juce::Slider positionSlider;//follows audio progress and can be used to jump to time positions

    juce::AudioProcessorValueTreeState apvts;

private:

       

    juce::AudioFormatReader* formatReader{nullptr};

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicPlayerAudioProcessor)
};
