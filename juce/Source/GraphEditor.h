/*
  ==============================================================================

    GraphEditor.h
    Created: 3 Mar 2019 6:20:37pm
    Author:  Pelle Juul  Christensen

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <functional>
#include "Node.h"

enum GraphEditorMode
{
    None,
    AddNode,
    AddString,
    ConnectNodes,
    ExciteNode
};

class GraphEditor : public Component, private Timer
{
public:
    std::function<void()> onModeChanged;
    std::function<void()> onNoteSelected;

    GraphEditor(std::vector<Node*> *nodes);
    
    GraphEditorMode getMode() { return currentMode; };
    
    void setMode(GraphEditorMode newMode);
    void setAddStringModeNumNodes(int value) { numNodesInString = value; } ;
    Node *getSelectedNode() { return selectedNode; };

    virtual void timerCallback() override;
    virtual void paint(Graphics &g) override;
    virtual bool keyStateChanged (bool isDown) override;
    virtual void mouseDown(const MouseEvent &event) override;
    virtual void mouseMove(const MouseEvent &event) override;

private:
    void paintNode(Graphics &g, float x, float y, float radius);

    Node *getNodeUnderCursor();

    GraphEditorMode currentMode;
    std::vector<Node*> *nodes;
    float nodeRadius = 5;
    
    Node *selectedNode = nullptr;
    
    // The node selected with in connect nodes mode.
    Node *connectNodeNode = nullptr;
    
    /// The first node of the string when in AddString mode. If this is
    /// not null it means that we're building a string.
    Node *stringFirstNode = nullptr;
    
    /// The number of nodes in the stirng to be added in AddStringMode.
    int numNodesInString = 10;
};
