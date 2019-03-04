/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GraphicalAudioProcessor::GraphicalAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

GraphicalAudioProcessor::~GraphicalAudioProcessor()
{
}

//==============================================================================
const String GraphicalAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GraphicalAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GraphicalAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GraphicalAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GraphicalAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GraphicalAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GraphicalAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GraphicalAudioProcessor::setCurrentProgram (int index)
{
}

const String GraphicalAudioProcessor::getProgramName (int index)
{
    return {};
}

void GraphicalAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void GraphicalAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void GraphicalAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GraphicalAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GraphicalAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto channel0 = buffer.getWritePointer(0);

    float wavespeed = 100.0;
    float independentDampening = 0.0001;
    float dependentDampening = 0.0001;
    
    float k = 1.0 / getSampleRate();
    float k2 = k * k;
    float h = 1.0 / 100.0;
    float rh2 = 1.0 / (h * h);
    float wavespeed2 = wavespeed * wavespeed;
    
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        for (int n = 0; n < nodes.size(); n++)
        {
            auto node = nodes.at(n);
            float u = node->value;
            float u1 = node->valuePrev;
            float dxx = rh2 * node->computeDxx();
            float dxx1 = node->dxxPrev;

            float y = (1.0 / (1.0 + k * independentDampening)) *
            (
                k2 * wavespeed2 * dxx +
                k * independentDampening * u1 +
                k * 2.0 * dependentDampening * (dxx - dxx1) +
                2.0 * u -
                u1 +
                k2 * node->force
            );
            node->valueTemp = y;
            node->dxxPrev = dxx;
        }

        for (int n = 0; n < nodes.size(); n++)
        {
            nodes.at(n)->finishUpdate();
        }

        if (nodes.size() > 0)
        {
            channel0[i] = nodes[0]->value;
        }
        else
        {
            channel0[i] = 0;
        }
    }

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 1; channel < totalNumInputChannels; ++channel)
    {
        buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());

        // ..do something to the data...
    }
}

//==============================================================================
bool GraphicalAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* GraphicalAudioProcessor::createEditor()
{
    return new GraphicalAudioProcessorEditor (*this);
}

//==============================================================================
void GraphicalAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GraphicalAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GraphicalAudioProcessor();
}
