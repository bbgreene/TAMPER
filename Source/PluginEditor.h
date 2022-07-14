/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../Source/bbg_gui/Dial.h"
#include "../Source/bbg_gui/Toggle.h"
#include "../Source/bbg_gui/PushButton.h"
#include "../Source/bbg_gui/Menu.h"
#include "../Source/bbg_gui/Label.h"

//==============================================================================
/**
*/
class TAMPERAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TAMPERAudioProcessorEditor (TAMPERAudioProcessor&);
    ~TAMPERAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    // DIALS
    bbg_gui::bbg_Dial out { " dB", -24.0, 24.0, 0.1, 0.0, 0.0 };
    bbg_gui::bbg_Dial mix { " %", 0.0, 1.0, 0.01, 1.0, 0.0 };
    
    // BORDERS
    juce::GroupComponent modelBorder;
    juce::GroupComponent cabBorder;
    juce::GroupComponent limiterBorder;
    juce::GroupComponent outBorder;
    
    // LABELS
    bbg_gui::bbg_dialLabel olumay { "Olumay dsp" };
    bbg_gui::bbg_dialLabel tamperTitle { "T A M P E R" };
    bbg_gui::bbg_dialLabel tamperVersion { "version 1.1" };
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TAMPERAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TAMPERAudioProcessorEditor)
};
