/*
  ==============================================================================

    GraphEditor.cpp
    Created: 3 Mar 2019 6:20:37pm
    Author:  Pelle Juul  Christensen

  ==============================================================================
*/

#include "GraphEditor.h"
#include "Node.h"
#include <vector>

GraphEditor::GraphEditor(std::vector<Node*> *nodes, GraphicalAudioProcessor *processor)
{
    this->nodes = nodes;
    this->processor = processor;
    currentMode = GraphEditorMode::None;
    setWantsKeyboardFocus(true);
    startTimerHz(30);
}

void GraphEditor::timerCallback()
{
    repaint();
}


void GraphEditor::paint(Graphics &g)
{
    g.setColour(Colours::white);
    g.fillRect(0, 0, getWidth(), getHeight());
    
    g.setColour(Colours::black);
    
    // Draw connections between nodes
    for (auto *node : *nodes)
    {
        for (auto *other : node->connections)
        {
            g.drawLine(node->x, node->y, other->x, other->y);
        }
    }
    
    auto mouseXY = getMouseXYRelative();
    auto *nodeUnderCursor = getNodeUnderCursor();
    
    // Draw line from node to mouse if in connect mode
    if (currentMode == GraphEditorMode::ConnectNodes && connectNodeNode != nullptr)
    {
        g.drawLine(connectNodeNode->x, connectNodeNode->y, mouseXY.getX(), mouseXY.getY());
    }
    else if (currentMode == GraphEditorMode::AddNode)
    {
        g.setColour(Colours::grey);
        g.fillEllipse(
            mouseXY.x - nodeRadius,
            mouseXY.y - nodeRadius,
            2.0 * nodeRadius,
            2.0 * nodeRadius);
    }
    
    g.setColour(Colours::black);

    for (auto *node : *nodes)
    {
        if (node->isDirichlet)
        {
            g.setColour(Colours::orange);
        }
        else if (node->isNeumann)
        {
            g.setColour(Colours::green);
        }
        else
        {
            g.setColour(Colour(0xFF282422));
        }
        
        float radius = fmin(10, fmax(0, nodeRadius + (nodeRadius / 2.0) * node->value));
        
        if (node->getOutputLevel() >= 0.001)
        {
            float outputRadius = radius * (1.0 + node->getOutputLevel());
            g.setColour(Colour(0xFFE34234));
            g.fillEllipse(
                node->x - outputRadius,
                node->y - outputRadius,
                2.0 * outputRadius,
                2.0 * outputRadius);
            g.setColour(Colour(0xFF282422));
        }
        
        if (node->getInputLevel() >= 0.001)
        {
            float inputRadius = radius * (1.0 + node->getInputLevel());
            g.setColour(Colour(0xFF37BE2C));
            g.fillEllipse(
                node->x - inputRadius,
                node->y - inputRadius,
                2.0 * inputRadius,
                2.0 * inputRadius);
            g.setColour(Colour(0xFF282422));
        }
        
        g.fillEllipse(
            node->x - radius,
            node->y - radius,
            2.0 * radius,
            2.0 * radius);
    }
    
    if (currentMode == GraphEditorMode::ExciteNode)
    {
        if (nodeUnderCursor != nullptr)
        {
            g.setColour(Colours::red);
            g.fillEllipse(
                nodeUnderCursor->x - nodeRadius,
                nodeUnderCursor->y - nodeRadius,
                2.0 * nodeRadius,
                2.0 * nodeRadius);
        }
    }
    
    if (selectedNode != nullptr)
    {
        g.setColour(Colours::lightblue);
        g.fillEllipse(
            selectedNode->x - nodeRadius,
            selectedNode->y - nodeRadius,
            2.0 * nodeRadius,
            2.0 * nodeRadius);
    }
    
    if (currentMode == GraphEditorMode::AddString && stringFirstNode != nullptr)
    {
        float dx = (mouseXY.getX() - stringFirstNode->x) / (float)(numNodesInString - 1);
        float dy = (mouseXY.getY() - stringFirstNode->y) / (float)(numNodesInString - 1);
        
        g.setColour(Colours::grey);
        for (int i = 0; i < numNodesInString; i++)
        {
            float x = stringFirstNode->x + i * dx;
            float y = stringFirstNode->y + i * dy;
            paintNode(g, x, y, nodeRadius);
        }
        
        for (int i = 1; i < numNodesInString; i++)
        {
            float x1 = stringFirstNode->x + i * dx;
            float y1 = stringFirstNode->y + i * dy;
            float x2 = stringFirstNode->x + (i-1) * dx;
            float y2 = stringFirstNode->y + (i-1) * dy;
            g.drawLine(x1, y1, x2, y2);
        }
    }
    
    auto stateString = "";
    
    switch (currentMode)
    {
    case GraphEditorMode::None:
        stateString = "Select";
        break;
    case GraphEditorMode::AddNode:
        stateString = "Add nodes";
        break;
    case GraphEditorMode::AddString:
        stateString = "Add string";
        break;
    case GraphEditorMode::ConnectNodes:
        stateString = "Connect nodes";
        break;
    case GraphEditorMode::ExciteNode:
        stateString = "Excite node";
        break;
    default:
        stateString = "Unknown mode (this should not happen)";
        break;
    }
    
    int stateStringMargin = 10;
    
    g.setColour(Colours::black);
    g.drawText(
        stateString,
        stateStringMargin,
        getHeight() - g.getCurrentFont().getHeight() - stateStringMargin,
        200,
        30,
        Justification::left);

     repaint();
}

void GraphEditor::setMode(GraphEditorMode newMode)
{
    currentMode = newMode;
    
    if (currentMode == GraphEditorMode::ConnectNodes)
    {
        connectNodeNode = nullptr;
    }
    
    if (currentMode == GraphEditorMode::AddString)
    {
        stringFirstNode = nullptr;
    }
    
    if (onModeChanged)
    {
        onModeChanged();
    }
    
    repaint();
}

bool GraphEditor::keyStateChanged (bool isDown)
{
    if (KeyPress::isKeyCurrentlyDown(KeyPress::escapeKey))
    {
        goBack();
    }
    
    return false;
}

void GraphEditor::mouseDown(const MouseEvent &event)
{
    auto mouseXY = getMouseXYRelative();
    auto nodeUnderCursor = getNodeUnderCursor();

    if (event.mods.isRightButtonDown())
    {
        goBack();
    }
    else if (currentMode == GraphEditorMode::None)
    {
        if (nodeUnderCursor != nullptr)
        {
            selectedNode = nodeUnderCursor;
            if (onNoteSelected)
            {
                onNoteSelected();
            }
            
            repaint();
        }
    }
    else if (currentMode == GraphEditorMode::AddNode)
    {
        if (nodes != nullptr)
        {
            auto *node = new Node();
            node->x = mouseXY.getX();
            node->y = mouseXY.getY();
            nodes->push_back(node);
            
            if (nodes->size() == 1)
            {
                node->isDirichlet = true;
            }
            
            repaint();
        }
    }
    else if (currentMode == GraphEditorMode::AddString)
    {
        if (stringFirstNode == nullptr)
        {
            auto *node = new Node();
            node->x = mouseXY.getX();
            node->y = mouseXY.getY();
            stringFirstNode = node;
        }
        else
        {
            // TODO: Add nodes in string.
            float dx = (mouseXY.getX() - stringFirstNode->x) / (float)(numNodesInString - 1);
            float dy = (mouseXY.getY() - stringFirstNode->y) / (float)(numNodesInString - 1);
            
            auto *prevNode = stringFirstNode;
            nodes->push_back(stringFirstNode);
            
            for (int i = 1; i < numNodesInString; i++)
            {
                auto *node = new Node();
                node->x = stringFirstNode->x + i * dx;
                node->y = stringFirstNode->y + i * dy;
                prevNode->connect(node);
                nodes->push_back(node);
                prevNode = node;
            }
            
            stringFirstNode = nullptr;
        }
    }
    else if (currentMode == GraphEditorMode::ConnectNodes)
    {
        if (nodeUnderCursor != nullptr)
        {
            if (connectNodeNode == nullptr)
            {
                connectNodeNode = nodeUnderCursor;
            }
            else
            {
                for (auto *node : *nodes)
                {
                    node->value = 0.0;
                    node->valuePrev = 0.0;
                    node->dxxPrev = 0.0;
                }
            
                connectNodeNode->connect(nodeUnderCursor);
                
                if (event.mods.isShiftDown())
                {
                    connectNodeNode = nodeUnderCursor;
                }
                else
                {
                    connectNodeNode = nullptr;
                }
            }
        }
    }
    else if (currentMode == GraphEditorMode::ExciteNode && nodeUnderCursor != nullptr)
    {
        for (auto *node : *nodes)
        {
            node->value = 0.0;
            node->valuePrev = 0.0;
            node->dxxPrev = 0.0;
        }
        
        processor->wavespeed = processor->referenceWavespeed;
    
        nodeUnderCursor->value = 1.0;
        nodeUnderCursor->valuePrev = 1.0;
    }
    
    repaint();
}

void GraphEditor::mouseMove(const MouseEvent &event)
{
    if (currentMode == GraphEditorMode::ConnectNodes &&
        connectNodeNode != nullptr)
    {
        repaint();
    }
    else if (currentMode == GraphEditorMode::AddNode)
    {
        repaint();
    }
    else if (currentMode == GraphEditorMode::AddString)
    {
        repaint();
    }
    else if (currentMode == GraphEditorMode::ExciteNode)
    {
        repaint();
    }
}

Node *GraphEditor::getNodeUnderCursor()
{
    auto mouseXY = getMouseXYRelative();
    
    for (auto *node : *nodes)
    {
        float d = mouseXY.getDistanceFrom(Point<int>(node->x, node->y));
        
        if (d < nodeRadius)
        {
            return node;
        }
    }
    
    return nullptr;
}

void GraphEditor::paintNode(Graphics &g, float x, float y, float radius)
{
    g.fillEllipse(
        x - radius,
        y - radius,
        2.0 * radius,
        2.0 * radius);
}

void GraphEditor::goBack()
{
    repaint();

    if (currentMode == GraphEditorMode::ConnectNodes && connectNodeNode != nullptr)
    {
        connectNodeNode = nullptr;
    }
    else if (currentMode == GraphEditorMode::AddString && stringFirstNode != nullptr)
    {
        stringFirstNode = nullptr;
    }
    else if (currentMode == GraphEditorMode::None && selectedNode != nullptr)
    {
        selectedNode = nullptr;
        onNoteSelected();
    }
    else
    {
        setMode(GraphEditorMode::None);
    }
}
