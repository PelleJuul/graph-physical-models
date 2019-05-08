#include "Node.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <functional>

void Node::connect(Node *node)
{
    node->connections.push_back(this);
    connections.push_back(node);
}

void Node::connectExternal(Connection *connection)
{
    externalConnections.push_back(connection);
}

void Node::disconnect(Node *node)
{
    std::vector<Node*>::iterator it = std::find(connections.begin(), connections.end(), node);
    
    while (it != connections.end())
    {
        connections.erase(it);
        it = std::find(connections.begin(), connections.end(), node);
    }
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

void Node::setWavespeed(float value)
{
    auto f = [&](Node *node)
    {
        node->wavespeed = value;
    };
    
    visitConnected(f);
}

void Node::visitConnected(std::function<void(Node*)> f)
{
    f(this);
    std::vector<Node*> visited;
    visited.push_back(this);

    for (auto *n : connections)
    {
        n->visitConnectedInner(f, &visited);
    }
}

void Node::setIgnoreMidi(bool value)
{
    auto f = [&](Node *node)
    {
        node->midiDisabled = value;
    };
    
    visitConnected(f);
}

void Node::visitConnectedInner(std::function<void(Node*)> f, std::vector<Node*> *visited)
{
    f(this);
    visited->push_back(this);

    for (auto *n : connections)
    {
        if (std::find(visited->begin(), visited->end(), n) == visited->end())
        {
            n->visitConnectedInner(f, visited);
        }
    }
}
