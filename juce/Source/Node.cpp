#include "node.h"
#include <stdio.h>
#include <vector>

void Node::connect(Node *node)
{
    node->connections.push_back(this);
    connections.push_back(node);
}

float Node::sumConnected()
{
    float sum = 0;

    for (auto *n : connections)
    {
        sum += n->value;
    }

    return sum;
}

float Node::computeDxx()
{
    float sum = sumConnected();
    
    float dxx = 0;
    
    if (isDirichlet)
    {
        dxx = sum - 2.0 * connections.size() * value;
    }
    else if (isNeumann)
    {
        dxx = 2.0 * sum - 2.0 * connections.size() * value;
    }
    else
    {
        dxx = sum - connections.size() * value;
    }
    
    return dxx;
}

void Node::finishUpdate()
{
    valuePrev = value;
    value = valueTemp;
    force = 0;
}
