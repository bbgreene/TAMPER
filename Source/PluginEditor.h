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
    
    // DIALS & BUTTONS & MENUS
    bbg_gui::bbg_Dial out { "", -24.0, 24.0, 0.1, 0.0, 0.0 };
    bbg_gui::bbg_Dial mix { "", 0.0, 100.0, 0.01, 100.0, 0.0 };
    bbg_gui::bbg_Toggle phase { "Phase" };
    
    bbg_gui::bbg_Dial thres { "", -10.0, 0.0, 0.01, 0.0, 0.0 };
    bbg_gui::bbg_Dial rel { "", 1.0, 1000.0, 0.01, 100.0, 0.0 };
    bbg_gui::bbg_PushButton limiterOn { "On" };
    
    bbg_gui::bbg_PushButton cabOn { "On" };
    bbg_gui::bbg_Dial cabMix { "", 0.0, 100.0, 0.01, 100.0, 0.0 };
    bbg_gui::bbg_Menu cabType;
    
    bbg_gui::bbg_PushButton bypassFilters { "On" };
    bbg_gui::bbg_Dial hiPass { "", 20, 2000.0, 1.0, 20.0, 0.0 };
    bbg_gui::bbg_Dial loPass { "", 5000.0, 20000.0, 1.0, 20000.0, 0.0 };

    bbg_gui::bbg_PushButton driveOn { "On" };
    bbg_gui::bbg_Dial drive { "", 0.0, 24.0, 0.1, 0.0, 0.0 };
    bbg_gui::bbg_Menu modelType;
    
    bbg_gui::bbg_PushButton oS { "Oversample" };
    
    // BORDERS
    juce::GroupComponent modelBorder;
    juce::GroupComponent cabBorder;
    juce::GroupComponent limiterBorder;
    juce::GroupComponent outBorder;
    
    // LABELS
    bbg_gui::bbg_dialLabel outLabel { "Output" };
    bbg_gui::bbg_dialLabel mixLabel { "Mix" };
    
    bbg_gui::bbg_dialLabel thresLabel { "Threshold" };
    bbg_gui::bbg_dialLabel relLabel { "Release" };
    
    bbg_gui::bbg_dialLabel cabMixLabel { "Cab Mix" };
    bbg_gui::bbg_menuLabel cabTypeLabel { "Cab Type" };
    
    bbg_gui::bbg_dialLabel hiPassLabel { "Pre HiPass" };
    bbg_gui::bbg_dialLabel loPassLabel { "Post LoPass" };
    
    bbg_gui::bbg_dialLabel driveLabel { "Drive" };
    bbg_gui::bbg_menuLabel modelTypeLabel { "Model Type" };
    
    bbg_gui::bbg_dialLabel olumay { "Olumay dsp" };
    bbg_gui::bbg_dialLabel tamperTitle { "T A M P E R" };
    bbg_gui::bbg_dialLabel tamperVersion { "version 1.1" };
    
    // ATTACHMENTS
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> filterOnOffAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hiPassAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> loPassAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> driveOnAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modelTypeAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> cabOnAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cabMixAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> cabTypeAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> limiterOnAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> relAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> phaseAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> oSAttachement;

    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TAMPERAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TAMPERAudioProcessorEditor)
};
