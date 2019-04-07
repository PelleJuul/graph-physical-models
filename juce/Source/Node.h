#pragma once

#include <vector>

class Node
{
public:
    std::vector<Node*> connections;
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
    float sumConnected();
    float computeDxx();
    void finishUpdate();
    void setOutputLevel(float value) { outputLevel = value; };
    float getOutputLevel() { return outputLevel; };
    
private:
    float outputLevel = 0.0;
};
