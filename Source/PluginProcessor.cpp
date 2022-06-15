/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TAMPERAudioProcessor::TAMPERAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout()), overSamplingModule(2, 2, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR)
#endif
{
    treeState.addParameterListener("oversample", this);
}

TAMPERAudioProcessor::~TAMPERAudioProcessor()
{
    treeState.removeParameterListener("oversample", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout TAMPERAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.reserve(9);
    
    auto pOSToggle = std::make_unique<juce::AudioParameterBool>("oversample", "Oversample", false);
    
    params.push_back(std::move(pOSToggle));
    
    return { params.begin(), params.end() };
}

void TAMPERAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == "oversample")
    {
        osToggle = newValue;
    }
}

//==============================================================================
const juce::String TAMPERAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TAMPERAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TAMPERAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TAMPERAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TAMPERAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TAMPERAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TAMPERAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TAMPERAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TAMPERAudioProcessor::getProgramName (int index)
{
    return {};
}

void TAMPERAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TAMPERAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    
    //Oversampling prep
    osToggle = *treeState.getRawParameterValue("oversample");
    overSamplingModule.initProcessing(samplesPerBlock);
}

void TAMPERAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TAMPERAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void TAMPERAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //My two audioblocks
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::AudioBlock<float> upSampledBlock (buffer);
    
    //if oversampling on...
    if(osToggle)
    {   //increase sample rate
        upSampledBlock = overSamplingModule.processSamplesUp(block);
        
        for(int sample = 0; sample < upSampledBlock.getNumSamples(); ++sample)
        {
            for (int ch = 0; ch < upSampledBlock.getNumChannels(); ++ch)
            {
                float* data = upSampledBlock.getChannelPointer(ch);
                
                data[sample] *= 1.0;
            }
        }
        //decrease sample rate back down
        overSamplingModule.processSamplesDown(block);
    }
    
    //if oversampling is off...
    else
    {
        for(int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            for (int ch = 0; ch < block.getNumChannels(); ++ch)
            {
                float* data = block.getChannelPointer(ch);
                
                data[sample] *= 1.0;
            }
        }
    }
    
}

//==============================================================================
bool TAMPERAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TAMPERAudioProcessor::createEditor()
{
//    return new TAMPERAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void TAMPERAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TAMPERAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TAMPERAudioProcessor();
}
