#pragma once

#include <vector>

#include "Node.h"
#include "Material.h"
#include "TrussElement.h"

class Model {
public:

    void addNode(const Node& node);

    void addElement(const TrussElement& element);

    const std::vector<Node>& getNodes() const;

    const std::vector<TrussElement>& getElements() const;

private:

    std::vector<Node> nodes;

    std::vector<TrussElement> elements;
};