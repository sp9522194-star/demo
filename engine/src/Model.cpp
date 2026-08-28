#include "Model.h"

void Model::addNode(const Node& node) {
    nodes.push_back(node);
}

void Model::addElement(const TrussElement& element) {
    elements.push_back(element);
}

const std::vector<Node>& Model::getNodes() const {
    return nodes;
}

const std::vector<TrussElement>& Model::getElements() const {
    return elements;
}