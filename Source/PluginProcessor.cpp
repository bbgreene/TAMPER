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
    treeState.addParameterListener("drive", this);
    treeState.addParameterListener("model", this);
    treeState.addParameterListener("high pass", this);
    treeState.addParameterListener("low pass", this);
    treeState.addParameterListener("mix", this);
    treeState.addParameterListener("out", this);
    treeState.addParameterListener("convolve", this);
}

TAMPERAudioProcessor::~TAMPERAudioProcessor()
{
    treeState.removeParameterListener("oversample", this);
    treeState.removeParameterListener("drive", this);
    treeState.removeParameterListener("model", this);
    treeState.removeParameterListener("high pass", this);
    treeState.removeParameterListener("low pass", this);
    treeState.removeParameterListener("mix", this);
    treeState.removeParameterListener("out", this);
    treeState.removeParameterListener("convolve", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout TAMPERAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    juce::StringArray disModels = { "Soft", "Hard", "Tube", "Saturation" };
    
    params.reserve(9);
    
    auto pOSToggle = std::make_unique<juce::AudioParameterBool>("oversample", "Oversample", false);
    auto pDrive = std::make_unique<juce::AudioParameterFloat>("drive", "Drive", 0.0, 24.0, 0.0);
    auto pModels = std::make_unique<juce::AudioParameterChoice>("model", "Model", disModels, 0);
    auto pHighPass = std::make_unique<juce::AudioParameterFloat>("high pass", "High Pass", juce::NormalisableRange<float> (20.0, 20000.0, 1.0, 0.22), 20.0);
    auto pLowPass = std::make_unique<juce::AudioParameterFloat>("low pass", "Low Pass", juce::NormalisableRange<float> (20.0, 20000.0, 1.0, 0.22), 20000.0);
    auto pMix = std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0, 1.0, 1.0);
    auto pOut = std::make_unique<juce::AudioParameterFloat>("out", "Out", juce::NormalisableRange<float> (-24.0f, 24.0f, 0.01f, 1.0f), 0.00f);
    auto pConv = std::make_unique<juce::AudioParameterBool>("convolve", "Convole", false);
    
    params.push_back(std::move(pOSToggle));
    params.push_back(std::move(pDrive));
    params.push_back(std::move(pModels));
    params.push_back(std::move(pHighPass));
    params.push_back(std::move(pLowPass));
    params.push_back(std::move(pMix));
    params.push_back(std::move(pOut));
    params.push_back(std::move(pConv));
    
    return { params.begin(), params.end() };
}

void TAMPERAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == "oversample")
    {
        osToggle = newValue;
    }
    if (parameterID == "drive")
    {
        dBInput = newValue;
        rawInput = juce::Decibels::decibelsToGain(dBInput);
    }
    if (parameterID == "model")
    {
        switch (static_cast<int>(newValue))
        {
            case 0: disModel = DisModels::kSoft; break;
            case 1: disModel = DisModels::KHard; break;
            case 2: disModel = DisModels::KTube; break;
            case 3: disModel = DisModels::KSat; break;
        }
    }
    if (parameterID == "high pass")
    {
        highPassFilter = newValue;
        highPassFilterPre.setCutoffFrequency(highPassFilter);
    }
    if (parameterID == "low pass")
    {
        lowPassFilter = newValue;
        lowPassFilterPost.setCutoffFrequency(lowPassFilter);
    }
    if (parameterID == "mix")
    {
        mix = newValue;
    }
    smoothOutput.setTargetValue(juce::Decibels::decibelsToGain(treeState.getRawParameterValue("out")->load()));
    if (parameterID == "convolve")
    {
        ConvolveOn = newValue;
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
    overSamplingModule.reset();
    
    ConvolveOn = *treeState.getRawParameterValue("convolve");
    convolution.reset();
    convolution.prepare(spec);
    
    //function to load IR from computer
//    convolution.loadImpulseResponse(juce::File ("/Users/programming/Desktop/cassette_recorder.wav"), juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0, juce::dsp::Convolution::Normalise::yes);
    
    //function to load IR from binary
    convolution.loadImpulseResponse(BinaryData::ABLCR_M2S_1_Loud_aif, BinaryData::ABLCR_M2S_1_Loud_aifSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0, juce::dsp::Convolution::Normalise::no);
    
    rawInput = juce::Decibels::decibelsToGain(static_cast<float>(*treeState.getRawParameterValue("drive")));
    
    disModel = static_cast<DisModels>(treeState.getRawParameterValue("model")->load());
    
    highPassFilterPre.prepare(spec);
    highPassFilterPre.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    highPassFilterPre.setCutoffFrequency(treeState.getRawParameterValue("high pass")->load());
    
    lowPassFilterPost.prepare(spec);
    lowPassFilterPost.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    lowPassFilterPost.setCutoffFrequency(treeState.getRawParameterValue("low pass")->load());
    
    mix = treeState.getRawParameterValue("mix")->load();
    
    smoothOutput.reset(sampleRate, 0.1f);
    smoothOutput.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(treeState.getRawParameterValue("out")->load()));
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
    
    smoothOutput.setTargetValue(juce::Decibels::decibelsToGain(treeState.getRawParameterValue("out")->load()));
    
    //My two audioblocks
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::AudioBlock<float> upSampledBlock (buffer);
    
    //if oversampling on...
    if(osToggle)
    {
        // high pass filter pre distortion
        for(int channel = 0; channel < block.getNumChannels(); ++channel)
        {
            float* data = block.getChannelPointer(channel);

            for (int sample = 0; sample < block.getNumSamples(); ++sample)
            {
                auto dry = data[sample];
                data[sample] = highPassFilterPre.processSample(channel, data[sample]);
                data[sample] = (1.0 - mix.getNextValue()) * dry + mix.getNextValue() * data[sample];
            }
        }
        
        //increase sample rate
        upSampledBlock = overSamplingModule.processSamplesUp(block);
        
        for(int channel = 0; channel < upSampledBlock.getNumChannels(); ++channel)
        {
            float* data = upSampledBlock.getChannelPointer(channel);

            for (int sample = 0; sample < upSampledBlock.getNumSamples(); ++sample)
            {
                drySignal = data[sample]; //dry signal stored in variable

                switch(disModel)
                {
                    case DisModels::kSoft: data[sample] = softClipData(data[sample]); break;
                    case DisModels::KHard: data[sample] = hardClipData(data[sample]); break;
                    case DisModels::KTube: data[sample] = tubeData(data[sample]); break;
                    case DisModels::KSat: data[sample] = saturationData(data[sample]); break;
                }
                
                blendSignal = (1.0 - mix.getNextValue()) * drySignal + mix.getNextValue() * data[sample];
                data[sample] = blendSignal;
            }
        }
        //decrease sample rate back down
        overSamplingModule.processSamplesDown(block);
        
        // low pass filter post distortion
        for(int channel = 0; channel < block.getNumChannels(); ++channel)
        {
            float* data = block.getChannelPointer(channel);

            for (int sample = 0; sample < block.getNumSamples(); ++sample)
            {
                auto dry = data[sample];
                data[sample] = lowPassFilterPost.processSample(channel, data[sample]);
                data[sample] = ((1.0 - mix.getNextValue()) * dry + mix.getNextValue() * data[sample]) * smoothOutput.getNextValue();
            }
        }
    }
    
    //if oversampling is off...
    else
    {
        for(int channel = 0; channel < block.getNumChannels(); ++channel)
        {
            float* data = block.getChannelPointer(channel);
            
            for (int sample = 0; sample < block.getNumSamples(); ++sample)
            {
                drySignal = data[sample]; //dry signal stored in variable
                auto highPassed = highPassFilterPre.processSample(channel, data[sample]); // high pass filter pre distortion
                
                switch(disModel)
                {
                    case DisModels::kSoft: data[sample] = softClipData(highPassed); break;
                    case DisModels::KHard: data[sample] = hardClipData(highPassed); break;
                    case DisModels::KTube: data[sample] = tubeData(highPassed); break;
                    case DisModels::KSat: data[sample] = saturationData(highPassed); break;
                }
                
                data[sample] = lowPassFilterPost.processSample(channel, data[sample]); // low pass filter post distortion

                blendSignal = (1.0 - mix.getNextValue()) * drySignal + mix.getNextValue() * data[sample];
                data[sample] = blendSignal * smoothOutput.getNextValue();
            }
        }
    }
    
    //Convolution process
    juce::dsp::ProcessContextReplacing<float> context (block);
    if (ConvolveOn) convolution.process(context);
}

// softclip function (rounded)
float TAMPERAudioProcessor::softClipData(float sample)
{
    sample *= rawInput * 1.6;
    return piDivisor * std::atan(sample);
}

// hardclip function (any sample above 1 or -1 will be squared)
float TAMPERAudioProcessor::hardClipData(float sample)
{
    sample *= rawInput;
    
    if(std::abs(sample) > 1.0)
    {
        sample *= 1.0 / std::abs(sample);
    }
    return sample;
}

// tube function (postive values will hardclip, negative values will softclip)
float TAMPERAudioProcessor::tubeData(float sample)
{
    sample *= rawInput * 1.6;
    
    if (sample < 0.0)
    {
        sample = piDivisor * std::atan(sample);
    }
    else if (std::abs(sample) > 1.0)
    {
        // if true then this will output 1 (or -1)
        sample = hardClipData(sample);
    }
    sample = piDivisor * std::atan(sample);
    return sample;
}

// saturation function
float TAMPERAudioProcessor::saturationData(float sample)
{
    sample *= rawInput;
    if (sample < thresh)
    {
        sample = sample;
    }
    
    else if (sample > thresh)
    {
        sample = thresh + (sample - thresh) / (1.0 + std::pow((( sample - thresh) / (1.0 - thresh)), 2.0));
    }
    
    else if (sample > 1.0)
    {
        sample = (thresh + 1.0) * 0.5;
    }
    
    if (sample < -0.8)
    {
        sample = -0.8;
    }
    return sample;
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
