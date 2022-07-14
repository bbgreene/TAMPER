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
    // DIALS & BUTTONS
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
    
    //DIAL LABEL ATTACHMENTS
    outLabel.attachToComponent(&out, false);
    mixLabel.attachToComponent(&mix, false);
    thresLabel.attachToComponent(&thres, false);
    relLabel.attachToComponent(&rel, false);
    
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
    auto bottomRowY = getHeight() * 0.683966;
    auto middleRowY = getHeight() * 0.383879;
    
    auto phaseSize = getWidth() * 0.0833333;
    auto phaseX = outBorder.getX() + (getWidth() * 0.0417);
    auto phaseY = getHeight() * 0.193093;
    
    auto buttonWidth = getWidth() * 0.075;
    auto buttonHeight = getHeight() * 0.0667;
    auto limiterOnX = limiterBorder.getX() + (getWidth() * 0.044);
    auto limiterOnY = getHeight() * 0.23;
    
    out.setBounds(outBorder.getX() + smallDialGap, bottomRowY, smallDialSize, smallDialSize);
    mix.setBounds(outBorder.getX() + smallDialGap, middleRowY, smallDialSize, smallDialSize);
    phase.setBounds(phaseX, phaseY, phaseSize, phaseSize);
    
    rel.setBounds(limiterBorder.getX() + smallDialGap, bottomRowY, smallDialSize, smallDialSize);
    thres.setBounds(limiterBorder.getX() + smallDialGap, middleRowY, smallDialSize, smallDialSize);
    limiterOn.setBounds(limiterOnX, limiterOnY, buttonWidth, buttonHeight);
    
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
