/*
  ==============================================================================

    Connection.cpp
    Created: 1 May 2019 11:17:18pm
    Author:  Pelle Juul  Christensen

  ==============================================================================
*/

#include "Connection.h"
#include "Node.h"

Connection::Connection(Node *nodeA, Node *nodeB)
{
    this->nodeA = nodeA;
    this->nodeB = nodeB;
    
    linearCoef = 10;
    nonLinearCoef = 1000;
    dampening = 1;
}

void Connection::computeForce(float k)
{
    float linearCoef2 = linearCoef * linearCoef;
    float nonLinearCoef4 = nonLinearCoef * nonLinearCoef * nonLinearCoef * nonLinearCoef;
    float etaNow = nodeA->value - nodeB->value;
    float a = (2 * dampening) / k + linearCoef2 + nonLinearCoef4 * etaNow * etaNow;
    float p = -2 / a;
    float r = ((2 * dampening) / k - linearCoef2 - nonLinearCoef4 * etaNow * etaNow) / a;
    float etaNext = p * computedForce + r * etaPrev;

    float avg = (etaNext - etaPrev);
    computedForce = -linearCoef2 * 0.5 * avg - nonLinearCoef4 * etaNow * etaNow * 0.5 * avg - 2 * dampening * (1.0 / (2.0 * k)) * (avg);
    nodeA->force += computedForce;
    nodeB->force -= computedForce;
    etaPrev = etaNow;
}

void Connection::getCenter(float *x, float *y)
{
    *x = nodeA->x + 0.5 * (nodeB->x - nodeA->x);
    *y = nodeA->y + 0.5 * (nodeB->y - nodeA->y);
}
