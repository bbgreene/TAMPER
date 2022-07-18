/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TAMPERAudioProcessorEditor::TAMPERAudioProcessorEditor (TAMPERAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // DIALS, BUTTONS, MENUS & PARAMETER ATTACHMENTS
    
    phaseAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "phase", phase);
    mixAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "main Mix", mix);
    outputAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "out", out);
    
    phase.setToggleStyle(bbg_gui::bbg_Toggle::ToggleStyle::kPhaseToggle);
    addAndMakeVisible(phase);
    mix.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    mix.textFromValueFunction = nullptr;
    addAndMakeVisible(mix);
    out.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    out.textFromValueFunction = nullptr;
    addAndMakeVisible(out);
    
    limiterOnAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "limiter", limiterOn);
    thresAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "thres", thres);
    relAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "release", rel);
    
    limiterOn.setClickingTogglesState(true);
    addAndMakeVisible(limiterOn);
    thres.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    thres.textFromValueFunction = nullptr;
    addAndMakeVisible(thres);
    rel.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    rel.textFromValueFunction = nullptr;
    addAndMakeVisible(rel);
    
    cabOnAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "amp sim", cabOn);
    cabMixAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "amp sim mix", cabMix);
    cabTypeAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, "ampsim type", cabType);
    
    cabOn.setClickingTogglesState(true);
    addAndMakeVisible(cabOn);
    cabMix.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    cabMix.textFromValueFunction = nullptr;
    addAndMakeVisible(cabMix);
    cabType.setText("A Ribbon");
    cabType.addItem("A Ribbon", 1);
    cabType.addItem("A Dyn", 2);
    cabType.addItem("B Cond", 3);
    cabType.addItem("C Ribbon", 4);
    cabType.addItem("D Bass 1", 5);
    cabType.addItem("D Bass 2", 6);
    addAndMakeVisible(cabType);
    
    filterOnOffAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "filtersOnOff", bypassFilters);
    hiPassAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "high pass", hiPass);
    loPassAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "low pass", loPass);
    driveOnAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "driveOn", driveOn);
    modelTypeAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, "model", modelType);
    driveAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "drive", drive);
    
    bypassFilters.setClickingTogglesState(true);
    addAndMakeVisible(bypassFilters);
    hiPass.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(hiPass);
    loPass.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(loPass);
    oS.setClickingTogglesState(true);
    addAndMakeVisible(oS);
    driveOn.setClickingTogglesState(true);
    addAndMakeVisible(driveOn);
    drive.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    drive.textFromValueFunction = nullptr;
    addAndMakeVisible(drive);
    modelType.setText("Soft Clip");
    modelType.addItem("Soft Clip", 1);
    modelType.addItem("Hard Clip", 2);
    modelType.addItem("Saturation", 3);
    modelType.addItem("Broken", 4);
    addAndMakeVisible(modelType);
    
    oSAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "oversample", oS);
    
    //DIAL LABEL ATTACHMENTS
    outLabel.attachToComponent(&out, false);
    mixLabel.attachToComponent(&mix, false);
    thresLabel.attachToComponent(&thres, false);
    relLabel.attachToComponent(&rel, false);
    cabMixLabel.attachToComponent(&cabMix, false);
    cabTypeLabel.attachToComponent(&cabType, false);
    hiPassLabel.attachToComponent(&hiPass, false);
    loPassLabel.attachToComponent(&loPass, false);
    driveLabel.attachToComponent(&drive, false);
    modelTypeLabel.attachToComponent(&modelType, false);
    
    // BORDERS
    modelBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::lightgrey);
    modelBorder.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::whitesmoke);
    modelBorder.setTextLabelPosition(juce::Justification::centred);
    modelBorder.setText("MODEL");
    addAndMakeVisible(modelBorder);
    
    cabBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::lightgrey);
    cabBorder.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::whitesmoke);
    cabBorder.setTextLabelPosition(juce::Justification::centred);
    cabBorder.setText("CAB SIM");
    addAndMakeVisible(cabBorder);
    
    limiterBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::lightgrey);
    limiterBorder.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::whitesmoke);
    limiterBorder.setTextLabelPosition(juce::Justification::centred);
    limiterBorder.setText("LIMITER");
    addAndMakeVisible(limiterBorder);
    
    outBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::lightgrey);
    outBorder.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::whitesmoke);
    outBorder.setTextLabelPosition(juce::Justification::centred);
    outBorder.setText("OUT");
    addAndMakeVisible(outBorder);
    
    // TITLE
    tamperTitle.setFont(juce::Font (30.0f, juce::Font::plain));
    tamperTitle.setJustificationType(juce::Justification::centredLeft);
    tamperTitle.setColour(juce::Label::textColourId, juce::Colours::mintcream);
    addAndMakeVisible(tamperTitle);
    
    tamperVersion.setFont(juce::Font (16.0f, juce::Font::plain));
    tamperVersion.setJustificationType(juce::Justification::centredLeft);
    tamperVersion.setColour(juce::Label::textColourId, juce::Colours::lightslategrey.brighter());
    addAndMakeVisible(tamperVersion);
    
    olumay.setFont(juce::Font (16.0f, juce::Font::plain));
    olumay.setJustificationType(juce::Justification::centredLeft);
    olumay.setColour(juce::Label::textColourId, juce::Colours::coral.brighter());
    addAndMakeVisible(olumay);
    
    // RESIZING
    setResizable(true, true);
    setResizeLimits(600, 300, 900, 450); // * 1.5
    getConstrainer()->setFixedAspectRatio(2.0);
    
    setSize (600, 300);
}

TAMPERAudioProcessorEditor::~TAMPERAudioProcessorEditor()
{
}

//==============================================================================
void TAMPERAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Rectangle<int> background = getLocalBounds();
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colours::steelblue, getHeight() - getHeight(), juce::Colours::steelblue.darker(), getHeight()));
    g.fillRect(background);
    
    //Line
    auto lineX = getWidth() * 0.197;
    auto lineYone = getHeight() * 0.22;
    auto lineYtwo = getHeight() * 0.92;
    g.setColour (juce::Colours::lightgrey);
    
    juce::Line<float> line (juce::Point<float> (lineX, lineYone),
                            juce::Point<float> (lineX, lineYtwo));

     g.drawLine (line, 1.0f);
}

void TAMPERAudioProcessorEditor::resized()
{
    auto leftMarginGap = getWidth() * 0.0333;
    auto modelBorderWidth = getWidth() * 0.3333;
    auto smallBorderWidth = getWidth() * 0.1666666666666;
    auto borderHeight = getHeight() *  0.8333;
    auto borderY = getHeight() * 0.14;

    modelBorder.setBounds(leftMarginGap, borderY, modelBorderWidth, borderHeight);
    cabBorder.setBounds(modelBorder.getRight() + leftMarginGap, borderY, smallBorderWidth, borderHeight);
    limiterBorder.setBounds(cabBorder.getRight() + leftMarginGap, borderY, smallBorderWidth, borderHeight);
    outBorder.setBounds(limiterBorder.getRight() + leftMarginGap, borderY, smallBorderWidth, borderHeight);
    
    auto tamperTitleX = leftMarginGap;
    auto tamperTitleY = getHeight() * -0.016;
    auto tamperTitleWidth = getWidth() * 0.284883;
    auto tamperTitleHeight = getHeight() * 0.17;
    auto versionX = getWidth() * 0.284608;
    auto versionY = getHeight() * 0.0383032;
    auto versionWidth = getWidth() * 0.169246;
    auto versionHeight = getHeight() * 0.068969;
    auto olumayX = getWidth() * 0.795;
    
    tamperTitle.setBounds(tamperTitleX, tamperTitleY, tamperTitleWidth, tamperTitleHeight);
    tamperVersion.setBounds(versionX, versionY, versionWidth, versionHeight);
    olumay.setBounds(olumayX, versionY, versionWidth, versionHeight);
    
    auto smallDialSize = getWidth() * 0.1166; //70
    auto smallDialGap = getWidth() * 0.025; //15
    auto middleRowY = getHeight() * 0.383879;
    auto bottomRowY = getHeight() * 0.683966;
    
    auto phaseSize = getWidth() * 0.0833333;
    auto phaseX = outBorder.getX() + (getWidth() * 0.0417);
    auto phaseY = getHeight() * 0.193093;
    
    auto buttonWidth = getWidth() * 0.075;
    auto buttonHeight = getHeight() * 0.0667;
    auto limiterOnX = limiterBorder.getX() + (getWidth() * 0.044);
    auto buttonsY = getHeight() * 0.23;
    
    auto cabOnX = cabBorder.getX() + (getWidth() * 0.044);
    
    auto cabTypeX = cabBorder.getX() + (getWidth() * 0.013);
    auto cabTypeY = getHeight() * 0.7;
    auto cabTypeWidth = getWidth() * 0.142794;
    
    auto bypassFiltersX = modelBorder.getX() * 2.5;
    auto filtersX = modelBorder.getX() * 1.80059;
    auto driveOnX = cabBorder.getX() * 0.61;
    auto driveX = modelBorder.getX() * 6.95028;
    auto modelTypeX = modelBorder.getX() * 6.47;
    auto modelTypeWidth = getWidth() * 0.149;// 0.1333333; //80

    out.setBounds(outBorder.getX() + smallDialGap, bottomRowY, smallDialSize, smallDialSize);
    mix.setBounds(outBorder.getX() + smallDialGap, middleRowY, smallDialSize, smallDialSize);
    phase.setBounds(phaseX, phaseY, phaseSize, phaseSize);
    
    rel.setBounds(limiterBorder.getX() + smallDialGap, bottomRowY, smallDialSize, smallDialSize);
    thres.setBounds(limiterBorder.getX() + smallDialGap, middleRowY, smallDialSize, smallDialSize);
    limiterOn.setBounds(limiterOnX, buttonsY, buttonWidth, buttonHeight);
    
    cabMix.setBounds(cabBorder.getX() + smallDialGap, middleRowY, smallDialSize, smallDialSize);
    cabOn.setBounds(cabOnX, buttonsY, buttonWidth, buttonHeight);
    cabType.setBounds(cabTypeX, cabTypeY, cabTypeWidth, buttonHeight);
    
    bypassFilters.setBounds(bypassFiltersX, buttonsY, buttonWidth, buttonHeight);
    hiPass.setBounds(filtersX, middleRowY, smallDialSize, smallDialSize);
    loPass.setBounds(filtersX, bottomRowY, smallDialSize, smallDialSize);
    driveOn.setBounds(driveOnX, buttonsY, buttonWidth, buttonHeight);
    drive.setBounds(driveX, middleRowY, smallDialSize, smallDialSize);
    modelType.setBounds(modelTypeX, cabTypeY, modelTypeWidth, buttonHeight);
    
    auto oSX = getWidth() * 0.229;
    auto oSY = getHeight() * 0.82;
    auto oSWidth = getWidth() * 0.102191;
    
    oS.setBounds(oSX, oSY, oSWidth, buttonHeight);
    
    // big border = 200
    // small borders = 100
    // total borders = 200+100+100+100 = 500
    //gaps and margin = 20 * 5
    //total x = 600
    
    // top margin = 35 for TAMPER
    // bottom = 15 for Olumay dsp
    
    
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
