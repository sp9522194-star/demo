#include "Model.h"

void Model::addNode(const Node& node) {
    nodes.push_back(node);
}

const std::vector<Node>& Model::getNodes() const {
    return nodes;
}