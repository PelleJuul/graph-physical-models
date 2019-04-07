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
    TextButton connectNodeButton;
    TextButton exciteNodeButton;

    Component toplevelInfo;
    Label stiffnessLabel;
    Slider stiffnessSlider;
    Label brightnessLabel;
    Slider brightnessSlider;
    Label decayLabel;
    Slider decaySlider;

    Component nodeInfo;
    Label boundaryConditionLabel;
    ToggleButton dirichletButton;
    ToggleButton neumannButton;
    
    Component addStringInfo;
    Label addStringNumNodesLabel;
    Slider addStringNumNodesSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicalAudioProcessorEditor)
};
