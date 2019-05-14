/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GraphFile.h"
#include <functional>
#include <iostream>
#include <fstream>

//==============================================================================
GraphicalAudioProcessorEditor::GraphicalAudioProcessorEditor (GraphicalAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), graphEditor(&p.nodes, &p.connections, &p)
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
    
    connectGraphButton.setButtonText("Connect graphs");
    connectGraphButton.setTooltip("Connect multiple subgraphs");
    connectGraphButton.onClick = [&]()
    {
        graphEditor.setMode(GraphEditorMode::AddExternalConnection);
    };
    addAndMakeVisible(connectGraphButton);
    
    exciteNodeButton.setButtonText("Excite node");
    exciteNodeButton.setTooltip("Excite a node to produce sound");
    exciteNodeButton.onClick = [&]()
    {
        graphEditor.setMode(GraphEditorMode::ExciteNode);
    };
    addAndMakeVisible(exciteNodeButton);
    
    resetButton.setButtonText("Reset");
    resetButton.setTooltip("Reset the position of all nodes.");
    resetButton.onClick = [&]()
    {
        for (Node *n : processor.nodes)
        {
            n->value = 0;
            n->valuePrev = 0;
            n->valueTemp = 0;
        }
    };
    addAndMakeVisible(resetButton);
    
    saveButton.setButtonText("Save");
    saveButton.setTooltip("Save model to file");
    saveButton.onClick = [&]()
    {
        std::string text = serialzeNodes(processor.nodes);
        std::cout << text;
        
        FileChooser chooser("Save graph", File::getSpecialLocation(File::userHomeDirectory), "*.graph");
        
        if (chooser.browseForFileToSave(true))
        {
            std::ofstream os;
            std::string filename = chooser.getResult().getFullPathName().toStdString();
            os.open(filename);
            os << text;
            os.close();
        }
    };
    addAndMakeVisible(saveButton);

    loadButton.setButtonText("Load");
    loadButton.setTooltip("Load model from file");
    loadButton.onClick = [&]()
    {
        std::string text = serialzeNodes(processor.nodes);
        std::cout << text;
        
        FileChooser chooser("Load graph", File::getSpecialLocation(File::userHomeDirectory), "*.graph");
        
        if (chooser.browseForFileToOpen())
        {
            std::ifstream is;
            std::string filename = chooser.getResult().getFullPathName().toStdString();
            is.open(filename);
            std::stringstream ss;
            ss << is.rdbuf();
            processor.nodes.clear();
            deserialzeNodes(ss.str(), &processor.nodes);
            is.close();
        }
    };
    addAndMakeVisible(loadButton);
    
    
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
    decaySlider.setValue(15);
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
    nodeOutputSlider.setRange(0, 400);
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
    
    // CONNECTION INFO
    
    connectionLinearLabel.setText("Linear", NotificationType::dontSendNotification);
    connectionInfo.addAndMakeVisible(connectionLinearLabel);
    
    connectionLinearSlider.setRange(1, 200);
    connectionLinearSlider.setNumDecimalPlacesToDisplay(2);
    connectionLinearSlider.setValue(100);
    connectionLinearSlider.onValueChange = [&]()
    {
        if (graphEditor.getSelectedConnection() != nullptr)
        {
            graphEditor.getSelectedConnection()->setLinearCoef(connectionLinearSlider.getValue());
        }
    };
    connectionInfo.addAndMakeVisible(connectionLinearSlider);

    connectionNonLinearLabel.setText("Non linear", NotificationType::dontSendNotification);
    connectionInfo.addAndMakeVisible(connectionNonLinearLabel);
    
    connectionNonLinearSlider.setRange(1, 200);
    connectionNonLinearSlider.setNumDecimalPlacesToDisplay(2);
    connectionNonLinearSlider.setValue(100);
    connectionNonLinearSlider.onValueChange = [&]()
    {
        if (graphEditor.getSelectedConnection() != nullptr)
        {
            graphEditor.getSelectedConnection()->setNonLinearCoed(connectionNonLinearSlider.getValue());
        }
    };
    connectionInfo.addAndMakeVisible(connectionNonLinearSlider);
    
    connectionDampeningLabel.setText("Dampening", NotificationType::dontSendNotification);
    connectionInfo.addAndMakeVisible(connectionDampeningLabel);
    
    connectionDampeningSlider.setRange(1, 200);
    connectionDampeningSlider.setNumDecimalPlacesToDisplay(2);
    connectionDampeningSlider.setValue(100);
    connectionDampeningSlider.onValueChange = [&]()
    {
        if (graphEditor.getSelectedConnection() != nullptr)
        {
            graphEditor.getSelectedConnection()->setDampening(connectionDampeningSlider.getValue());
        }
    };
    connectionInfo.addAndMakeVisible(connectionDampeningSlider);

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
    
    // printf("%f, %i\n", processor.currentLevel, (int)(10000 * processor.bowMovement));
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
    
    connectGraphButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    y += connectGraphButton.getHeight() + margin;
    
    exciteNodeButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    y += exciteNodeButton.getHeight() + margin;
    
    resetButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    y += resetButton.getHeight() + margin;
    
    saveButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    y += saveButton.getHeight() + margin;

    loadButton.setBounds(x, y, toolBoxWidth, toolBoxItemHeight);
    y += loadButton.getHeight() + margin;
    
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
        (graphEditor.getMode() == GraphEditorMode::None && (graphEditor.getSelectedNode() == nullptr && graphEditor.getSelectedConnection() == nullptr)) ||
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
    
    if (graphEditor.getSelectedConnection() != nullptr)
    {
        auto *c = graphEditor.getSelectedConnection();
        
        connectionLinearSlider.setValue(c->getLinearCoef());
        connectionNonLinearSlider.setValue(c->getNonLinearCoef());
        connectionDampeningSlider.setValue(c->getDampening());
    }
}
