/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

float sqr(float a)
{
    return a * a;
}

//==============================================================================
GraphicalAudioProcessor::GraphicalAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
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
    reset();
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

float GraphicalAudioProcessor::computeIntermediateNodeValue(float rh2, float k, float k2, Node *node)
{
    float u = node->value;
    float u1 = node->valuePrev;
    float dxx = rh2 * node->computeDxx();
    float dxx1 = node->dxxPrev;
    float wavespeed = node->getIgnoreMidi() ? node->getWavespeed() : freq * (node->getWavespeed() / 440.0);

    float y = (1.0 / (1.0 + k * independentDampening)) *
    (
        k2 * (wavespeed * wavespeed) * dxx +
        k * independentDampening * u1 +
        k * 2.0 * dependentDampening * (dxx - dxx1) +
        2.0 * u -
        u1
    );

    return y;
}

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
    
    float k = 1.0 / getSampleRate();
    float k2 = k * k;
    float h = 1.0 / 50.0;
    float rh2 = 1.0 / (h * h);
    
    MidiBuffer::Iterator iterator(midiMessages);
    MidiMessage message;
    int sampleNumber = midiMessages.getFirstEventTime();
    int lastSamplerNumber = midiMessages.getLastEventTime();
    // std::cout << model.hammerPosition0 << std::endl;
    
    while (iterator.getNextEvent(message, sampleNumber))
    {
        if (sampleNumber > lastSamplerNumber)
            break;
        
        if (message.isNoteOn())
        {
            float c = std::pow(2, 1.0 / 12.0);
            int note = message.getNoteNumber();
            freq = std::pow(c, note - 69) * 440;
            
            for (auto *node : nodes)
            {
                if (node->getInputLevel() > 0.01)
                {
                    float v = (message.getVelocity() / 127.0) * node->getInputLevel();
                    node->value = v;
                    node->valuePrev = v;
                }
            }
        }
    }
    
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        for (auto *c : connections)
        {
            float u1 = computeIntermediateNodeValue(rh2, k, k2, c->getNodeA());
            float u2 = computeIntermediateNodeValue(rh2, k, k2, c->getNodeB());
            float eta = c->getNodeA()->value - c->getNodeB()->value;
            float lin2 =  c->getLinearCoef() * c->getLinearCoef();
            float nlin4 = c->getNonLinearCoef() * c->getNonLinearCoef() * c->getNonLinearCoef() * c->getNonLinearCoef();
            float eta2 = eta * eta;
            float a = -(lin2 / 2.0) - ((nlin4 * eta2) / 2.0) - (c->getDampening() / k);
            float p = -2.0 / ((2.0 * c->getDampening()) / k + lin2 + nlin4*eta2);
            float r = ((2.0 * c->getDampening()) / k - lin2 - nlin4 * eta2) / ((2.0 * c->getDampening()) / k + lin2 + nlin4 * eta2);
            float K1 = k2 / (1 + k * independentDampening);
            float K2 = k2 / (1 + k * independentDampening);
            
            // -- Ways of computing Fc --
            
            // Pelle's first derivation
            // float fc = ((u1 - u2 - c->etaPrev) * a) / (1 + K1 - K2) * a;
            
            // Pelles second derivation using Bilbao's equations
            // float fc = (r * c->etaPrev - c->etaPrev * a) / (1 + p * a);
            
            // The definition from Silvin's paper
            // float fc = ((r * c->etaPrev) - (h * u1 - h * u2)) / (h * K1 + h * K2 - p);
            
            // Simple backwards difference scheme
            float fc = -lin2 * 0.5 * (eta - c->etaPrev) - nlin4 * eta2 * 0.5 * (eta - c->etaPrev) - 2 * c->getDampening() * (1.0 / k) * (eta - c->etaPrev);
            
            // No difference scheme at all (except the backwards difference operator)
            // float fc = -lin2 * eta - nlin4 * eta2 * eta - 2 * c->getDampening() * (1.0 / k) * (eta - c->etaPrev);
            
            fc = fc;
            printf("%f\n", fc);
            
            if (fc != fc)
            {
                int a = 42;
            }
            
            c->getNodeA()->force += fc;
            c->getNodeB()->force += -fc;
            c->etaPrev = eta;
        }
    
        for (int n = 0; n < nodes.size(); n++)
        {
            auto node = nodes.at(n);
            
            float y = computeIntermediateNodeValue(rh2, k, k2, node) + (1.0 / (1.0 + k * independentDampening)) * (k2 * node->force);
            
            node->valueTemp = y;
            node->dxxPrev = rh2 * node->computeDxx();;
        }

        float y = 0;
        
        for (int n = 0; n < nodes.size(); n++)
        {
            auto *node = nodes.at(n);
            node->finishUpdate();
            y += node->getOutputLevel() * node->value;
        }

        channel0[i] = y;
    }


    for (int channel = 1; channel < totalNumOutputChannels; ++channel)
    {
        buffer.copyFrom(channel, 0, buffer, 0, 0, buffer.getNumSamples());
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
