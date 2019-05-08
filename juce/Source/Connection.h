/*
  ==============================================================================

    Connection.h
    Created: 1 May 2019 11:17:18pm
    Author:  Pelle Juul  Christensen

  ==============================================================================
*/

#pragma once

class Node;

class Connection
{
    public:
    
    Connection(Node *nodeA, Node *nodeB);

    float getForceFor(Node *node);
    float getComputedForce() { return computedForce; };
    void computeForce(float k);
    
    void setLinearCoef(float value) { linearCoef = value; };
    float getLinearCoef() { return linearCoef; };
    void setNonLinearCoed(float value) { nonLinearCoef = value; };
    float getNonLinearCoef() { return nonLinearCoef; };
    void setDampening(float value) { dampening = value; };
    float getDampening() { return dampening; };
    
    Node *getNodeA() { return nodeA; };
    Node *getNodeB() { return nodeB; };
    
    void getCenter(float *x, float *y);
    
    private:
    Node *nodeA;
    Node *nodeB;
    float dnow = 0;
    float dprev = 0;
    float computedForce = 0;
    float linearCoef;
    float nonLinearCoef;
    float dampening;
};
