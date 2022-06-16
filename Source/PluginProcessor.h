/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class TAMPERAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    TAMPERAudioProcessor();
    ~TAMPERAudioProcessor() override;

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

    juce::AudioProcessorValueTreeState treeState;
    
private:
    
    //oversampling variable and module intialisation
    bool osToggle { false };
    juce::dsp::Oversampling<float> overSamplingModule;
    
    //Drive variables
    float dBInput { 0.0 };
    float rawInput { 1.0 };
    
    //Distortion models selection class
    enum class DisModels
    {
        kSoft,
        KHard,
        KTube
    };
    
    //Distortion models selection class instantiation
    DisModels disModel = DisModels::kSoft;
    
    //constant for distortion data
    static constexpr float piDivisor = 2.0 / juce::MathConstants<float>::pi;

    // distortion data
    float softClipData (float sample);
    float hardClipData (float sample);
    float tubeData (float sample);
    
    //parameter layout and change functions
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TAMPERAudioProcessor)
};
