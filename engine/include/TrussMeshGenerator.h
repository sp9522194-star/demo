#pragma once

#include <vector>

#include "Node.h"
#include "TrussElement.h"
#include "Material.h"

class TrussMeshGenerator {
public:

    static std::vector<Node> generateNodes(
        const Node& start,
        const Node& end,
        int divisions,
        int startNodeId
    );

    static std::vector<TrussElement> generateElements(
        const std::vector<Node>& nodes,
        const Material& material,
        int startElementId
    );
};