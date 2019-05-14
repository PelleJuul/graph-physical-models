/*
  ==============================================================================

    GraphFile.h
    Created: 14 May 2019 9:03:41pm
    Author:  Pelle Juul  Christensen

  ==============================================================================
*/

#pragma once

#include <vector>
#include <string>
#include "Node.h"

std::string serialzeNodes(const std::vector<Node*> nodes);

void deserialzeNodes(std::string text, std::vector<Node*> *out_nodes);
