/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <functional>

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
    
    addGridButton.setButtonText("Add grid");
    addGridButton.setTooltip("Add a grid to the model");
    addGridButton.onClick = [&]()
    {
        graphEditor.setMode(GraphEditorMode::AddGrid);
    };
    addAndMakeVisible(addGridButton);
    
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
    brightnessSlider.setValue(95);
    brightnessSlider.onValueChange = [&]()
    {
        processor.dependentDampening = 0.01 * (1.0 - brightnessSlider.getValue() / 100.0);
    };
    toplevelInfo.addAndMakeVisible(brightnessSlider);
    
    decayLabel.setText("Dampening", NotificationType::dontSendNotification);
    toplevelInfo.addAndMakeVisible(decayLabel);
    
    decaySlider.setRange(0, 100);
    decaySlider.setNumDecimalPlacesToDisplay(0);
    decaySlider.setValue(5);
    decaySlider.onValueChange = [&]()
    {
        processor.independentDampening = 30.0 * (decaySlider.getValue() / 100.0);
    };
    toplevelInfo.addAndMakeVisible(decaySlider);
    
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
    
    nodeWavespeedLabel.setText("Stiffness (pitch)", NotificationType::dontSendNotification);
    nodeInfo.addAndMakeVisible(nodeWavespeedLabel);
    
    nodeWavespeedSlider.setRange(1, 500);
    nodeWavespeedSlider.setNumDecimalPlacesToDisplay(2);
    nodeWavespeedSlider.setValue(100);
    nodeWavespeedSlider.onValueChange = [&]()
    {
        if (graphEditor.getSelectedNode() != nullptr)
        {
            graphEditor.getSelectedNode()->setWavespeed(nodeWavespeedSlider.getValue());
        }
    };
    nodeInfo.addAndMakeVisible(nodeWavespeedSlider);
    
    nodeIgnoreMidiButton.setButtonText("Ignore MIDI");
    nodeInfo.addAndMakeVisible(nodeIgnoreMidiButton);
    nodeIgnoreMidiButton.onStateChange = [&]()
    {
        if (graphEditor.getSelectedNode() != nullptr)
        {
            graphEditor.getSelectedNode()->setIgnoreMidi(nodeIgnoreMidiButton.getToggleState());
        }
    };
    
    addAndMakeVisible(nodeInfo);
    nodeInfo.setVisible(false);
    
    
    // ADD STRING INFO
    
    addStringNumNodesSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    addStringNumNodesSlider.setRange(2, 200);
    addStringNumNodesSlider.setNumDecimalPlacesToDisplay(0);
    addStringNumNodesSlider.setValue(5);
    addStringNumNodesSlider.onValueChange = [&]()
    {
        graphEditor.setAddStringModeNumNodes(addStringNumNodesSlider.getValueObject().getValue());
    };
    addStringInfo.addAndMakeVisible(addStringNumNodesSlider);
    
    addStringNumNodesLabel.setText("Number of nodes:", NotificationType::dontSendNotification);
    addStringInfo.addAndMakeVisible(addStringNumNodesLabel);
    
    addAndMakeVisible(addStringInfo);
    addStringInfo.setVisible(false);
    
    
    // ADD GRAPH INFO
    
    addGridNumRowsLabel.setText("Number of rows:", NotificationType::dontSendNotification);
    addGraphInfo.addAndMakeVisible(addGridNumRowsLabel);
    
    addGridNumRowsSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    addGridNumRowsSlider.setRange(2, 20);
    addGridNumRowsSlider.setNumDecimalPlacesToDisplay(0);
    addGridNumRowsSlider.setValue(5);
    addGridNumRowsSlider.onValueChange = [&]()
    {
        graphEditor.setAddGridModeNumRows(addGridNumRowsSlider.getValueObject().getValue());
    };
    addGraphInfo.addAndMakeVisible(addGridNumRowsSlider);
    
    addGraphNumColsLabel.setText("Number of columns:", NotificationType::dontSendNotification);
    addGraphInfo.addAndMakeVisible(addGraphNumColsLabel);
    
    addGraphNumColsSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    addGraphNumColsSlider.setRange(2, 20);
    addGraphNumColsSlider.setNumDecimalPlacesToDisplay(0);
    addGraphNumColsSlider.setValue(5);
    addGraphNumColsSlider.onValueChange = [&]()
    {
        graphEditor.setAddGridModeNumCols(addGraphNumColsSlider.getValueObject().getValue());
    };
    addGraphInfo.addAndMakeVisible(addGraphNumColsSlider);
    
    addAndMakeVisible(addGraphInfo);
    addGraphInfo.setVisible(false);
    
    
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
    
    auto setInfoSize = [&](Component &c)
    {
        c.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
        y += c.getHeight();
    };
    
    
    // TOOLBOX
    
    addNodeButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    y += addNodeButton.getHeight() + margin;
    
    addStringButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    y += addStringButton.getHeight() + margin;
    
    addGridButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    y += addGridButton.getHeight() + margin;
    
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
    setInfoSize(nodeInputSlider);
    y += margin;
    setInfoSize(nodeWavespeedLabel);
    setInfoSize(nodeWavespeedSlider);
    y += margin;
    setInfoSize(nodeIgnoreMidiButton);
    
    // ADD STRING INFO
    
    addStringInfo.setBounds(getWidth() - infoBoxWidth, 0, infoBoxWidth, getHeight());
    x = margin;
    y = 0;
    addStringNumNodesLabel.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    y += addStringNumNodesLabel.getHeight();
    addStringNumNodesSlider.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    
    // ADD GRID INFO
    
    addGraphInfo.setBounds(getWidth() - infoBoxWidth, 0, infoBoxWidth, getHeight());
    x = margin;
    y = 0;
    setInfoSize(addGridNumRowsLabel);
    setInfoSize(addGridNumRowsSlider);
    y += margin;
    setInfoSize(addGraphNumColsLabel);
    setInfoSize(addGraphNumColsSlider);
    
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
    addGraphInfo.setVisible(graphEditor.getMode() == GraphEditorMode::AddGrid);
    
    if (graphEditor.getSelectedNode() != nullptr)
    {
        auto *node = graphEditor.getSelectedNode();
        dirichletButton.setToggleState(graphEditor.getSelectedNode()->isDirichlet, NotificationType::dontSendNotification);
        neumannButton.setToggleState(graphEditor.getSelectedNode()->isNeumann, NotificationType::dontSendNotification);
        nodeOutputSlider.setValue(100.0 * graphEditor.getSelectedNode()->getOutputLevel());
        nodeInputSlider.setValue(100.0 * graphEditor.getSelectedNode()->getInputLevel());
        nodeWavespeedSlider.setValue(graphEditor.getSelectedNode()->getWavespeed());
        nodeIgnoreMidiButton.setToggleState(node->getIgnoreMidi(), NotificationType::dontSendNotification);
    }
}
