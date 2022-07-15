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
    // DIALS & BUTTONS & MENUS
    out.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(out);
    mix.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(mix);
    phase.setToggleStyle(bbg_gui::bbg_Toggle::ToggleStyle::kPhaseToggle);
    addAndMakeVisible(phase);
    
    thres.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(thres);
    rel.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(rel);
    limiterOn.setClickingTogglesState(true);
    addAndMakeVisible(limiterOn);
    
    cabOn.setClickingTogglesState(true);
    addAndMakeVisible(cabOn);
    cabMix.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(cabMix);
    cabType.setText("A Ribbon");
    cabType.addItem("A Ribbon", 1);
    cabType.addItem("A Dyn", 2);
    cabType.addItem("B Cond", 3);
    cabType.addItem("C Ribbon", 4);
    cabType.addItem("D Bass 1", 5);
    cabType.addItem("D Bass 2", 6);
    addAndMakeVisible(cabType);
    
    hiPass.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(hiPass);
    loPass.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(loPass);
    oS.setClickingTogglesState(true);
    addAndMakeVisible(oS);
    
    drive.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(drive);
    modelType.setText("Soft Clip");
    modelType.addItem("Soft Clip", 1);
    modelType.addItem("Hard Clip", 2);
    modelType.addItem("Tube Saturation", 3);
    modelType.addItem("Tube Broken", 4);
    addAndMakeVisible(modelType);
    
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
    tamperTitle.setColour(juce::Label::textColourId, juce::Colours::lightslategrey.brighter());
    addAndMakeVisible(tamperTitle);
    
    tamperVersion.setFont(juce::Font (16.0f, juce::Font::plain));
    tamperVersion.setJustificationType(juce::Justification::centredLeft);
    tamperVersion.setColour(juce::Label::textColourId, juce::Colours::mintcream);
    addAndMakeVisible(tamperVersion);
    
    olumay.setFont(juce::Font (16.0f, juce::Font::plain));
    olumay.setJustificationType(juce::Justification::centredLeft);
    olumay.setColour(juce::Label::textColourId, juce::Colours::navajowhite);
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
//    auto lineX = (getWidth() / 2) + ((getHeight() / 4) / 2);
//    auto lineYone = getHeight() * 0.6465;
//    auto lineYtwo = getHeight() * 0.7112;
//    g.setColour (CustomColours::blackGrey.brighter(0.8));
//    
//    juce::Line<float> line (juce::Point<float> (lineX, lineYone),
//                            juce::Point<float> (lineX, lineYtwo));
//
//     g.drawLine (line, 1.0f);
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
    auto tamperTitleY = getHeight() * -0.0290775;
    auto tamperTitleWidth = getWidth() * 0.284883;
    auto tamperTitleHeight = getHeight() * 0.17;
    auto versionX = getWidth() * 0.284608;
    auto versionY = getHeight() * -0.190713;
    auto versionWidth = getWidth() * 0.169246;
    auto versionHeight = getHeight() * 0.5;
    auto olumayX = getWidth() * 0.81;
    
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
    auto cabTypeWidth = getWidth() * 0.1333333; //80
    auto cabTypeX = cabBorder.getX() + (getWidth() * 0.017);
    auto cabTypeY = getHeight() * 0.752512;
    
    auto oSX = cabBorder.getX() * 0.58;
    auto filtersX = modelBorder.getX() * 1.80059;
    auto driveX = modelBorder.getX() * 7.16238;
    auto modelTypeX = modelBorder.getX() * 6.8623;

    out.setBounds(outBorder.getX() + smallDialGap, bottomRowY, smallDialSize, smallDialSize);
    mix.setBounds(outBorder.getX() + smallDialGap, middleRowY, smallDialSize, smallDialSize);
    phase.setBounds(phaseX, phaseY, phaseSize, phaseSize);
    
    rel.setBounds(limiterBorder.getX() + smallDialGap, bottomRowY, smallDialSize, smallDialSize);
    thres.setBounds(limiterBorder.getX() + smallDialGap, middleRowY, smallDialSize, smallDialSize);
    limiterOn.setBounds(limiterOnX, buttonsY, buttonWidth, buttonHeight);
    
    cabMix.setBounds(cabBorder.getX() + smallDialGap, middleRowY, smallDialSize, smallDialSize);
    cabOn.setBounds(cabOnX, buttonsY, buttonWidth, buttonHeight);
    cabType.setBounds(cabTypeX, cabTypeY, cabTypeWidth, buttonHeight);
    
    oS.setBounds(oSX, buttonsY, smallDialSize, buttonHeight);
    hiPass.setBounds(filtersX, middleRowY, smallDialSize, smallDialSize);
    loPass.setBounds(filtersX, bottomRowY, smallDialSize, smallDialSize);
    drive.setBounds(driveX, middleRowY, smallDialSize, smallDialSize);
    modelType.setBounds(modelTypeX, cabTypeY, cabTypeWidth, buttonHeight);
    
    
    
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
