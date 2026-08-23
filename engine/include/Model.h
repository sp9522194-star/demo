#pragma once

#include <vector>
#include "Node.h"
#include "Material.h"

class Model {
public:
    void addNode(const Node& node);

    const std::vector<Node>& getNodes() const;

private:
    std::vector<Node> nodes;
};