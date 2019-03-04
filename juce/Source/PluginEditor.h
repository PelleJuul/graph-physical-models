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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GraphicalAudioProcessor& processor;
    GraphEditor graphEditor;
    
    Component toolMenu;
    TextButton addNodeButton;
    TextButton connectNodeButton;
    TextButton exciteNodeButton;

    Component nodeInfo;
    ToggleButton dirichletButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicalAudioProcessorEditor)
};
