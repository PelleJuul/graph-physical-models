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

GraphEditor::GraphEditor(std::vector<Node*> *nodes)
{
    this->nodes = nodes;
    setWantsKeyboardFocus(true);
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
        else
        {
            g.setColour(Colours::black);
        }
        
        float radius = fmax(0, nodeRadius + (nodeRadius / 2.0) * node->value);
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
    
    auto stateString = "";
    
    switch (currentMode)
    {
    case GraphEditorMode::None:
        stateString = "Select";
        break;
    case GraphEditorMode::AddNode:
        stateString = "Add nodes";
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
}

void GraphEditor::setMode(GraphEditorMode newMode)
{
    currentMode = newMode;
    
    if (currentMode == GraphEditorMode::ConnectNodes)
    {
        connectNodeNode = nullptr;
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
        repaint();
    
        if (currentMode == GraphEditorMode::ConnectNodes && connectNodeNode != nullptr)
        {
            connectNodeNode = nullptr;
            return true;
        }
        else
        {
            setMode(GraphEditorMode::None);
            return true;
        }
    }
    
    return false;
}

void GraphEditor::mouseDown(const MouseEvent &event)
{
    auto mouseXY = getMouseXYRelative();
    auto nodeUnderCursor = getNodeUnderCursor();

    if (currentMode == GraphEditorMode::None)
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
    else if (currentMode == GraphEditorMode::ConnectNodes)
    {
        if (event.mods.isRightButtonDown())
        {
            connectNodeNode = nullptr;
        }
        else if (nodeUnderCursor != nullptr)
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
