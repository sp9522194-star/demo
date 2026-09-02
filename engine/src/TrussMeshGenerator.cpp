#include "TrussMeshGenerator.h"

#include <stdexcept>

std::vector<Node>
TrussMeshGenerator::generateNodes(
    const Node& start,
    const Node& end,
    int divisions,
    int startNodeId
) {
    if (divisions <= 0) {
        throw std::runtime_error(
            "Number of divisions must be greater than zero."
        );
    }

    std::vector<Node> nodes;

    double dx =
        (end.getX() - start.getX())
        / divisions;

    double dy =
        (end.getY() - start.getY())
        / divisions;

    for (int i = 0; i <= divisions; ++i) {

        double x =
            start.getX() + i * dx;

        double y =
            start.getY() + i * dy;

        Node node(
            startNodeId + i,
            x,
            y
        );

        nodes.push_back(node);
    }

    return nodes;
}

std::vector<TrussElement>
TrussMeshGenerator::generateElements(
    const std::vector<Node>& nodes,
    const Material& material,
    int startElementId
) {
    if (nodes.size() < 2) {
        throw std::runtime_error(
            "At least two nodes are required."
        );
    }

    std::vector<TrussElement> elements;

    for (std::size_t i = 0; i < nodes.size() - 1; ++i) {

        TrussElement element(
            startElementId + static_cast<int>(i),
            nodes[i],
            nodes[i + 1],
            material
        );

        elements.push_back(element);
    }

    return elements;
}