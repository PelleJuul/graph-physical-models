#pragma once

#include "Connection.h"
#include <vector>
#include <functional>

class Node
{
public:
    std::vector<Node*> connections;
    std::vector<Connection*> externalConnections;
    float x;
    float y;
    float value = 0;
    float valuePrev = 0;
    float valueTemp = 0;
    float dxxPrev = 0;
    float force = 0;
    bool isDirichlet;
    bool isNeumann;

    void connect(Node *node);
    void connectExternal(Connection *connection);
    void disconnect(Node *node);
    float sumConnected();
    float computeDxx();
    void finishUpdate();
    
    void setInputLevel(float value) { inputLevel = value; };
    float getInputLevel() { return inputLevel; };
    
    void setOutputLevel(float value) { outputLevel = value; };
    float getOutputLevel() { return outputLevel; };
    
    void setIgnoreMidi(bool value);
    bool getIgnoreMidi() { return midiDisabled; };
    
    void setWavespeed(float value);
    const float getWavespeed() { return wavespeed; };
    
    void visitConnected(std::function<void(Node*)> f);
    
private:
    float wavespeed = 100;
    float outputLevel = 0.0;
    float inputLevel = 0.0;
    bool midiDisabled = false;
    
    
    void setWavespeedInner(float value, std::vector<Node*> *visited);
    void visitConnectedInner(std::function<void(Node*)> f, std::vector<Node*> *visited);
};
