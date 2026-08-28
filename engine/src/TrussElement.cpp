#include "TrussElement.h"

#include <cmath>
#include <stdexcept>

TrussElement::TrussElement(
    int id,
    const Node& node1,
    const Node& node2,
    const Material& material
)
    : id(id),
      node1(node1),
      node2(node2),
      material(material) {
}

int TrussElement::getId() const {
    return id;
}

int TrussElement::getNode1Id() const {
    return node1.getId();
}

int TrussElement::getNode2Id() const {
    return node2.getId();
}

const Node& TrussElement::getNode1() const {
    return node1;
}

const Node& TrussElement::getNode2() const {
    return node2;
}

const Material& TrussElement::getMaterial() const {
    return material;
}

double TrussElement::getLength() const {

    double dx =
        node2.getX() - node1.getX();

    double dy =
        node2.getY() - node1.getY();

    double length =
        std::sqrt(
            dx * dx +
            dy * dy
        );

    if (length <= 0.0) {
        throw std::runtime_error(
            "Element has zero length."
        );
    }

    return length;
}

Eigen::Matrix4d
TrussElement::getStiffnessMatrix() const {

    double dx =
        node2.getX() - node1.getX();

    double dy =
        node2.getY() - node1.getY();

    double L =
        getLength();

    double c =
        dx / L;

    double s =
        dy / L;

    double E =
        material.getYoungsModulus();

    double A =
        material.getArea();

    double factor =
        (A * E) / L;

    Eigen::Matrix4d k;

    k <<
         c * c,  c * s, -c * c, -c * s,
         c * s,  s * s, -c * s, -s * s,
        -c * c, -c * s,  c * c,  c * s,
        -c * s, -s * s,  c * s,  s * s;

    return factor * k;
}