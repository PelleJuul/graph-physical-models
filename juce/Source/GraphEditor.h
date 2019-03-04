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
    ConnectNodes,
    ExciteNode
};

class GraphEditor : public Component
{
public:
    std::function<void()> onModeChanged;
    std::function<void()> onNoteSelected;

    GraphEditor(std::vector<Node*> *nodes);
    
    GraphEditorMode getMode() { return currentMode; };
    
    void setMode(GraphEditorMode newMode);
    Node *getSelectedNode() { return selectedNode; };

    virtual void paint(Graphics &g) override;
    virtual bool keyStateChanged (bool isDown) override;
    virtual void mouseDown(const MouseEvent &event) override;
    virtual void mouseMove(const MouseEvent &event) override;

private:
    Node *getNodeUnderCursor();

    GraphEditorMode currentMode;
    std::vector<Node*> *nodes;
    float nodeRadius = 8;
    
    Node *selectedNode = nullptr;
    
    // The node selected with in connect nodes mode.
    Node *connectNodeNode = nullptr;
};
