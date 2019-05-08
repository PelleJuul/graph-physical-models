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
#include "Connection.h"
#include "PluginProcessor.h"

enum GraphEditorMode
{
    None,
    AddNode,
    AddString,
    AddGrid,
    ConnectNodes,
    AddExternalConnection,
    ExciteNode
};

class GraphEditor : public Component, private Timer
{
public:
    std::function<void()> onModeChanged;
    std::function<void()> onNoteSelected;
    std::function<void()> onConnectionSelected;

    GraphEditor(std::vector<Node*> *nodes, std::vector<Connection*> *connections, GraphicalAudioProcessor *processor);
    
    GraphEditorMode getMode() { return currentMode; };
    
    void setMode(GraphEditorMode newMode);
    void setAddStringModeNumNodes(int value) { numNodesInString = value; } ;
    void setAddGridModeNumRows(int value) { addGridNumRows = value; };
    void setAddGridModeNumCols(int value) { addGridNumCols = value; };
    Node *getSelectedNode() { return selectedNode; };
    Connection *getSelectedConnection() { return selectedConnection; };

    virtual void timerCallback() override;
    virtual void paint(Graphics &g) override;
    virtual bool keyStateChanged (bool isDown) override;
    virtual void mouseDown(const MouseEvent &event) override;
    virtual void mouseMove(const MouseEvent &event) override;

private:
    GraphicalAudioProcessor *processor;
    void paintNode(Graphics &g, float x, float y, float radius);
    void goBack();

    Node *getNodeUnderCursor();
    Connection *getConnectionUnderCursor();

    GraphEditorMode currentMode;
    std::vector<Node*> *nodes;
    std::vector<Connection*> *connections;
    float nodeRadius = 5;
    
    Node *selectedNode = nullptr;
    Connection *selectedConnection = nullptr;
    
    // The node selected with in connect nodes mode.
    Node *connectNodeNode = nullptr;
    
    /// The first node of the string or grid when in AddString or AddGrid mode. If this is
    /// not null it means that we're building a string.
    Node *firstNodeWhenAdding = nullptr;
    
    /// The number of nodes in the stirng to be added in AddStringMode.
    int numNodesInString = 10;
    
    int addGridNumRows = 5;
    int addGridNumCols = 5;
};
