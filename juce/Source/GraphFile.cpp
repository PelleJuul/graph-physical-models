/*
  ==============================================================================

    GraphFile.cpp
    Created: 14 May 2019 9:03:41pm
    Author:  Pelle Juul  Christensen

  ==============================================================================
*/

#include "GraphFile.h"
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "Node.h"

std::string serialzeNodes(const std::vector<Node*> nodes)
{
    std::stringstream ss;
    
    for (auto *node : nodes)
    {
        ss <<
            "node " <<
            node->getWavespeed() << " "
            << node->getInputLevel() << " "
            << node->getOutputLevel() << " "
            << node->getIgnoreMidi() << " "
            << node->isNeumann << " "
            << node->isDirichlet << " "
            << node->x << " "
            << node->y << "\n";
    }
    
    for (int i = 0; i < nodes.size(); i++)
    {
        auto *node = nodes.at(i);
        
        for (auto *connectedNode : node->connections)
        {
            auto j = std::distance(nodes.begin(), std::find(nodes.begin(), nodes.end(), connectedNode));
            ss << "connect " << i << " " << j << "\n";
        }
    }
    
    return ss.str();
}

void deserialzeNodes(std::string text, std::vector<Node*> *out_nodes)
{
    std::stringstream ss;
    ss.str(text);
    
    while (ss)
    {
        std::string cmd;
        ss >> cmd;
        
        if (cmd == "node")
        {
            Node *node = new Node();
            float wavespeed;
            float inputLevel;
            float outputLevel;
            bool ignoreMidi;
            bool isNeumann;
            bool isDirichlet;
            float x;
            float y;
            ss >> wavespeed >> inputLevel >> outputLevel >> ignoreMidi >> isNeumann >> isDirichlet >> x >> y;
            node->setWavespeed(wavespeed);
            node->setInputLevel(inputLevel);
            node->setOutputLevel(outputLevel);
            node->setIgnoreMidi(ignoreMidi);
            node->isNeumann = isNeumann;
            node->isDirichlet = isDirichlet;
            node->x = x;
            node->y = y;
            out_nodes->push_back(node);
        }
        else if (cmd == "connect")
        {
            int i;
            int j;
            ss >> i >> j;
            out_nodes->at(i)->connections.push_back(out_nodes->at(j));
        }
        else if (cmd == "")
        {
            
        }
        else
        {
            throw std::runtime_error("Unexpected input " + cmd);
        }
    }
}
