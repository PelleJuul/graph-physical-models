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
    : AudioProcessorEditor (&p), processor (p), graphEditor(&p.nodes)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    addAndMakeVisible(graphEditor);
    
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
    
    dirichletButton.setButtonText("Dirichlet");
    nodeInfo.addAndMakeVisible(dirichletButton);
    dirichletButton.onStateChange = [&]()
    {
        if (graphEditor.getSelectedNode() != nullptr)
        {
            graphEditor.getSelectedNode()->isDirichlet = dirichletButton.getToggleState();
        }
    };
    
    addAndMakeVisible(nodeInfo);
    nodeInfo.setVisible(false);
    
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
    
    graphEditor.onNoteSelected = [&]()
    {
        if (graphEditor.getMode() == GraphEditorMode::None)
        {
            nodeInfo.setVisible(true);
            dirichletButton.setToggleState(graphEditor.getSelectedNode()->isDirichlet, NotificationType::dontSendNotification);
        }
    };
    
    graphEditor.onModeChanged = [&]()
    {
        nodeInfo.setVisible(false);
        addStringInfo.setVisible(false);
    
        if (graphEditor.getMode() == GraphEditorMode::AddString)
        {
            addStringInfo.setVisible(true);
        }
        else
        {
            addStringInfo.setVisible(true);
            addStringInfo.setVisible(false);
        }
    };
    
    setResizable(true, true);
    setSize (900, 600);
    
    auto *nodeA = new Node();
    nodeA->x = graphEditor.getWidth() / 2.0;
    nodeA->y = graphEditor.getHeight() / 2.0;
    nodeA->isDirichlet = true;
    auto *nodeB = new Node();
    nodeB->x = nodeA->x + 50;
    nodeB->y = nodeA->y;
    nodeB->connect(nodeA);
    processor.nodes.push_back(nodeB);
    processor.nodes.push_back(nodeA);
    graphEditor.repaint();
    
    graphEditor.toBack();
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
    
    addNodeButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    y += addNodeButton.getHeight() + margin;
    
    addStringButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    y += addStringButton.getHeight() + margin;
    
    connectNodeButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    y += connectNodeButton.getHeight() + margin;
    
    exciteNodeButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    
    graphEditor.setBounds(0, 0, getWidth() - infoBoxWidth, getHeight());
    
    // Node info
    nodeInfo.setBounds(getWidth() - infoBoxWidth, 0, infoBoxWidth, getHeight());
    x = margin;
    y = 0;
    dirichletButton.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    
    // Add string info
    addStringInfo.setBounds(getWidth() - infoBoxWidth, 0, infoBoxWidth, getHeight());
    x = margin;
    y = 0;
    addStringNumNodesLabel.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    y += addStringNumNodesLabel.getHeight();
    addStringNumNodesSlider.setBounds(x, y, infoBoxWidth, toolBoxItemHeight);
    
    graphEditor.toBack();
    nodeInfo.toFront(false);
}
