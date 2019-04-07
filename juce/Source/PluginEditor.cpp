/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GraphicalAudioProcessorEditor::GraphicalAudioProcessorEditor (GraphicalAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), graphEditor(&p.nodes, &p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    addAndMakeVisible(graphEditor);
    
    
    // TOOL BAR
    
    addNodeButton.setButtonText("Add node");
    addNodeButton.setTooltip("Add a new node to the graph");
    addNodeButton.onClick = [&]()
    {
        graphEditor.setMode(GraphEditorMode::AddNode);
    };
    addAndMakeVisible(addNodeButton);
    
    addStringButton.setButtonText("Add string");
    addStringButton.setTooltip("Add a string to the model");
    addStringButton.onClick = [&]()
    {
        graphEditor.setMode(GraphEditorMode::AddString);
    };
    addAndMakeVisible(addStringButton);
    
    connectNodeButton.setButtonText("Connect nodes");
    connectNodeButton.setTooltip("Connect nodes in the graph");
    connectNodeButton.onClick = [&]()
    {
        graphEditor.setMode(GraphEditorMode::ConnectNodes);
    };
    addAndMakeVisible(connectNodeButton);
    
    exciteNodeButton.setButtonText("Excite node");
    exciteNodeButton.setTooltip("Excite a node to produce sound");
    exciteNodeButton.onClick = [&]()
    {
        graphEditor.setMode(GraphEditorMode::ExciteNode);
    };
    addAndMakeVisible(exciteNodeButton);
    
    
    // SELECT NODE / TOPLEVEL TOOLBOX
    
    brightnessLabel.setText("Brightness", NotificationType::dontSendNotification);
    toplevelInfo.addAndMakeVisible(brightnessLabel);
    
    brightnessSlider.setRange(0, 100);
    brightnessSlider.setNumDecimalPlacesToDisplay(0);
    brightnessSlider.setValue(90);
    brightnessSlider.onValueChange = [&]()
    {
        processor.dependentDampening = 0.01 * (1.0 - brightnessSlider.getValue() / 100.0);
    };
    toplevelInfo.addAndMakeVisible(brightnessSlider);
    
    decayLabel.setText("Dampening", NotificationType::dontSendNotification);
    toplevelInfo.addAndMakeVisible(decayLabel);
    
    decaySlider.setRange(0, 100);
    decaySlider.setNumDecimalPlacesToDisplay(0);
    decaySlider.setValue(90);
    decaySlider.onValueChange = [&]()
    {
        processor.independentDampening = 30.0 * (decaySlider.getValue() / 100.0);
    };
    toplevelInfo.addAndMakeVisible(decaySlider);
    
    stiffnessLabel.setText("Stiffness (pitch)", NotificationType::dontSendNotification);
    toplevelInfo.addAndMakeVisible(stiffnessLabel);
    
    stiffnessSlider.setRange(1, 200);
    stiffnessSlider.setNumDecimalPlacesToDisplay(0);
    stiffnessSlider.setValue(100);
    stiffnessSlider.onValueChange = [&]()
    {
        processor.referenceWavespeed = stiffnessSlider.getValue();
    };
    toplevelInfo.addAndMakeVisible(stiffnessSlider);
    
    addAndMakeVisible(toplevelInfo);
    toplevelInfo.setVisible(false);
    

    // NODE INFO TOOLBOX
    
    boundaryConditionLabel.setText("Boundary condition:", NotificationType::dontSendNotification);
    nodeInfo.addAndMakeVisible(boundaryConditionLabel);
    
    dirichletButton.setButtonText("Dirichlet");
    nodeInfo.addAndMakeVisible(dirichletButton);
    dirichletButton.onStateChange = [&]()
    {
        if (graphEditor.getSelectedNode() != nullptr)
        {
            graphEditor.getSelectedNode()->isDirichlet = dirichletButton.getToggleState();
        }
    };
    
    neumannButton.setButtonText("Neumann");
    nodeInfo.addAndMakeVisible(neumannButton);
    neumannButton.onStateChange = [&]()
    {
        if (graphEditor.getSelectedNode() != nullptr)
        {
            graphEditor.getSelectedNode()->isNeumann = neumannButton.getToggleState();
        }
    };
    
    nodeOutputLabel.setText("Ouput volume:", NotificationType::dontSendNotification);
    nodeInfo.addAndMakeVisible(nodeOutputLabel);
    
    nodeOutputSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    nodeOutputSlider.setRange(0, 100);
    nodeOutputSlider.setNumDecimalPlacesToDisplay(0);
    nodeOutputSlider.setValue(0);
    nodeOutputSlider.onValueChange = [&]()
    {
        if (graphEditor.getSelectedNode() != nullptr)
        {
            graphEditor.getSelectedNode()->setOutputLevel(nodeOutputSlider.getValue() / 100.0);
        }
    };
    nodeInfo.addAndMakeVisible(nodeOutputSlider);

    nodeInputLabel.setText("Input volume:", NotificationType::dontSendNotification);
    nodeInfo.addAndMakeVisible(nodeInputLabel);
    
    nodeInputSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    nodeInputSlider.setRange(0, 100);
    nodeInputSlider.setNumDecimalPlacesToDisplay(0);
    nodeInputSlider.setValue(0);
    nodeInputSlider.onValueChange = [&]()
    {
        if (graphEditor.getSelectedNode() != nullptr)
        {
            graphEditor.getSelectedNode()->setInputLevel(nodeInputSlider.getValue() / 100.0);
        }
    };
    nodeInfo.addAndMakeVisible(nodeInputSlider);
    
    addAndMakeVisible(nodeInfo);
    nodeInfo.setVisible(false);
    
    
    // ADD STRING INFO
    
    addStringNumNodesSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    addStringNumNodesSlider.setRange(2, 200);
    addStringNumNodesSlider.setNumDecimalPlacesToDisplay(0);
    addStringNumNodesSlider.setValue(20);
    addStringNumNodesSlider.onValueChange = [&]()
    {
        graphEditor.setAddStringModeNumNodes(addStringNumNodesSlider.getValueObject().getValue());
    };
    addStringInfo.addAndMakeVisible(addStringNumNodesSlider);
    
    addStringNumNodesLabel.setText("Number of nodes:", NotificationType::dontSendNotification);
    addStringInfo.addAndMakeVisible(addStringNumNodesLabel);
    
    addAndMakeVisible(addStringInfo);
    addStringInfo.setVisible(false);
    
    
    // STATE CHANGE HANDLING
    
    graphEditor.onNoteSelected = [&]()
    {
        updateInfoBoxVisibilities();
    };
    
    graphEditor.onModeChanged = [&]()
    {
        updateInfoBoxVisibilities();
    };
    
    setResizable(true, true);
    setSize (900, 600);
    
    auto *nodeA = new Node();
    nodeA->x = graphEditor.getWidth() / 2.0;
    nodeA->y = graphEditor.getHeight() / 2.0;
    nodeA->isDirichlet = true;
    nodeA->setInputLevel(1.0);
    auto *nodeB = new Node();
    nodeB->x = nodeA->x + 50;
    nodeB->y = nodeA->y;
    nodeB->connect(nodeA);
    nodeB->setOutputLevel(1.0);
    processor.nodes.push_back(nodeB);
    processor.nodes.push_back(nodeA);
    graphEditor.repaint();
    
    graphEditor.toBack();
    
    updateInfoBoxVisibilities();
}

GraphicalAudioProcessorEditor::~GraphicalAudioProcessorEditor()
{
}

//==============================================================================
void GraphicalAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void GraphicalAudioProcessorEditor::resized()
{
    float toolBoxWidth = 150;
    float infoBoxWidth = 200;
    float toolBoxItemHeight = 30;
    float margin = 5.0;

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    float x = 0;
    float y = 0;
    
    
    // TOOLBOX
    
    addNodeButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    y += addNodeButton.getHeight() + margin;
    
    addStringButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    y += addStringButton.getHeight() + margin;
    
    connectNodeButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    y += connectNodeButton.getHeight() + margin;
    
    exciteNodeButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    
    graphEditor.setBounds(0, 0, getWidth() - infoBoxWidth, getHeight());
    
    
    // TOPLEVEL INFO
    
    toplevelInfo.setBounds(getWidth() - infoBoxWidth, 0, infoBoxWidth, getHeight());
    x = margin;
    y = 0;
    brightnessLabel.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    y += brightnessLabel.getHeight();
    
    brightnessSlider.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    y += brightnessSlider.getHeight() + margin;
    
    decayLabel.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    y += decayLabel.getHeight();
    
    decaySlider.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    y += decaySlider.getHeight() + margin;
    
    stiffnessLabel.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    y += stiffnessLabel.getHeight();
    
    stiffnessSlider.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    
    
    // NODE INFO
    
    nodeInfo.setBounds(getWidth() - infoBoxWidth, 0, infoBoxWidth, getHeight());
    x = margin;
    y = 0;
    boundaryConditionLabel.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    y += boundaryConditionLabel.getHeight();
    dirichletButton.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    y += dirichletButton.getHeight();
    neumannButton.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    y += neumannButton.getHeight() + margin;
    nodeOutputLabel.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    y += nodeOutputLabel.getHeight();
    nodeOutputSlider.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    y += nodeOutputSlider.getHeight() + margin;
    nodeInputLabel.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    y += nodeInputLabel.getHeight();
    nodeInputSlider.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    
    
    // ADD STRING INFO
    addStringInfo.setBounds(getWidth() - infoBoxWidth, 0, infoBoxWidth, getHeight());
    x = margin;
    y = 0;
    addStringNumNodesLabel.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    y += addStringNumNodesLabel.getHeight();
    addStringNumNodesSlider.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    
    graphEditor.toBack();
    nodeInfo.toFront(false);
}

void GraphicalAudioProcessorEditor::updateInfoBoxVisibilities()
{
    toplevelInfo.setVisible(
        (graphEditor.getMode() == GraphEditorMode::None && graphEditor.getSelectedNode() == nullptr) ||
        (graphEditor.getMode() == GraphEditorMode::ExciteNode));
    nodeInfo.setVisible(graphEditor.getMode() == GraphEditorMode::None && graphEditor.getSelectedNode() != nullptr);
    addStringInfo.setVisible(graphEditor.getMode() == GraphEditorMode::AddString);
    
    if (graphEditor.getSelectedNode() != nullptr)
    {
        dirichletButton.setToggleState(graphEditor.getSelectedNode()->isDirichlet, NotificationType::dontSendNotification);
        neumannButton.setToggleState(graphEditor.getSelectedNode()->isNeumann, NotificationType::dontSendNotification);
        nodeOutputSlider.setValue(100.0 * graphEditor.getSelectedNode()->getOutputLevel());
        nodeInputSlider.setValue(100.0 * graphEditor.getSelectedNode()->getInputLevel());
    }
}
