/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include "Node.h"
#include "Connection.h"

//==============================================================================
/**
*/
class GraphicalAudioProcessor  : public AudioProcessor
{
public:
    std::vector<Node*> nodes;
    std::vector<Connection*> connections;

    //==============================================================================
    GraphicalAudioProcessor();
    ~GraphicalAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    float computeIntermediateNodeValue(float rh2, float k, float k2, Node *node);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    float freq = 440;
    float independentDampening = 0.001;
    float dependentDampening = 0.001;
    float excitationLevel = 0;
    float currentLevel = 0;
    float bowMovement = 0.0;
    float mod = 0.0;
    int lastKey = 0;

private:

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicalAudioProcessor)
};
