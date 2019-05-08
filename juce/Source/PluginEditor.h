/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "GraphEditor.h"
#include <vector>

//==============================================================================
/**
*/
class GraphicalAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    GraphicalAudioProcessorEditor (GraphicalAudioProcessor&);
    ~GraphicalAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void updateInfoBoxVisibilities();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GraphicalAudioProcessor& processor;
    GraphEditor graphEditor;
    
    Component toolMenu;
    TextButton addNodeButton;
    TextButton addStringButton;
    TextButton addGridButton;
    TextButton connectNodeButton;
    TextButton connectGraphButton;
    TextButton exciteNodeButton;
    TextButton resetButton;

    Component toplevelInfo;
    Label brightnessLabel;
    Slider brightnessSlider;
    Label decayLabel;
    Slider decaySlider;

    Component nodeInfo;
    Label boundaryConditionLabel;
    ToggleButton dirichletButton;
    ToggleButton neumannButton;
    Label nodeOutputLabel;
    Slider nodeOutputSlider;
    Label nodeWavespeedLabel;
    Slider nodeWavespeedSlider;
    ToggleButton nodeIgnoreMidiButton;
    
    Component connectionInfo;
    Label connectionLinearLabel;
    Slider connectionLinearSlider;
    Label connectionNonLinearLabel;
    Slider connectionNonLinearSlider;
    Label connectionDampeningLabel;
    Slider connectionDampeningSlider;
    
    Label nodeInputLabel;
    Slider nodeInputSlider;
    
    Component addStringInfo;
    Label addStringNumNodesLabel;
    Slider addStringNumNodesSlider;
    
    Component addGraphInfo;
    Label addGridNumRowsLabel;
    Slider addGridNumRowsSlider;
    Label addGraphNumColsLabel;
    Slider addGraphNumColsSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicalAudioProcessorEditor)
};
