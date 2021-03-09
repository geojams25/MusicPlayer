/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <memory>
#include <vector>

//==============================================================================
MusicPlayerAudioProcessor::MusicPlayerAudioProcessor() : AudioProcessor(BusesProperties().withOutput("Out", juce::AudioChannelSet::stereo()))
                                                        ,apvts(*this,nullptr,"parameters",createParameters())

{
    formatManager.registerBasicFormats();
    transport.addChangeListener(this);
    transport.setPosition(0.0);

    state = stopped;//initial state of transport
    
    fileLoaded = false;//used by the pluginEditor. if false will disable all buttons (e.g on startup)

    

}

MusicPlayerAudioProcessor::~MusicPlayerAudioProcessor()
{

    transport.setSource(nullptr);
    formatReader = nullptr;
}


//==============================================================================
const juce::String MusicPlayerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MusicPlayerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MusicPlayerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MusicPlayerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MusicPlayerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MusicPlayerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MusicPlayerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MusicPlayerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MusicPlayerAudioProcessor::getProgramName (int index)
{
    return {};
}

void MusicPlayerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MusicPlayerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    //
    transport.prepareToPlay(samplesPerBlock, sampleRate);

}

void MusicPlayerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.

    transport.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MusicPlayerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MusicPlayerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
     
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    

        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto volume = apvts.getRawParameterValue("VOL");

    
    transport.getNextAudioBlock(juce::AudioSourceChannelInfo(buffer));
    transport.setGain(volume->load());
        

}

//==============================================================================
bool MusicPlayerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MusicPlayerAudioProcessor::createEditor()
{
    return new MusicPlayerAudioProcessorEditor (*this);
}

//==============================================================================
void MusicPlayerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MusicPlayerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


void MusicPlayerAudioProcessor::changeListenerCallback(juce::ChangeBroadcaster *source){

    if(source == &transport){
        if(transport.isPlaying())
            changeTransportState(playing);//see changeTransportState below...
        else if(state == stopping)
            changeTransportState(stopped);
        else if(state == pausing)
            changeTransportState(paused);

    }

}
void MusicPlayerAudioProcessor::changeTransportState(transportState newState){

    if(state != newState){

        state = newState;
        switch(state){
            case stopped:
                transport.setPosition(0.0);
                break;
            case starting:
                transport.start();
                break;
            case playing:
                //do nothing
                break;
            case stopping:
                transport.stop();
                break;
            case pausing:
                transport.stop();
                break;
            case paused:
                //do nothing
                break;
        }
    }

}

void MusicPlayerAudioProcessor::loadAudioFile(const juce::File& file){

    transport.stop();
    transport.setSource(nullptr);
    readerSource_ptr = nullptr;

    juce::AudioFormatReader* reader = formatManager.createReaderFor(file);
    currentlyLoadedFile = file;

    if(reader != nullptr){
        readerSource_ptr.reset(new juce::AudioFormatReaderSource(reader, true));
        transport.setSource(readerSource_ptr.get(),0,nullptr,reader->sampleRate);
        fileLoaded = true;
    }

}

juce::AudioProcessorValueTreeState::ParameterLayout MusicPlayerAudioProcessor::createParameters(){
        
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterFloat>("VOL","Vol",0.0f,1.0f,0.5f));

    return {params.begin(), params.end()};

}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MusicPlayerAudioProcessor();
}
