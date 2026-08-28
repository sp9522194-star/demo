#include "Node.h"

Node::Node(
    int id,
    double x,
    double y
)
    : id(id),
      x(x),
      y(y),
      fixedX(false),
      fixedY(false),
      forceX(0.0),
      forceY(0.0) {
}

int Node::getId() const {
    return id;
}

double Node::getX() const {
    return x;
}

double Node::getY() const {
    return y;
}

void Node::setFixedX(bool fixed) {
    fixedX = fixed;
}

void Node::setFixedY(bool fixed) {
    fixedY = fixed;
}

bool Node::isFixedX() const {
    return fixedX;
}

bool Node::isFixedY() const {
    return fixedY;
}

void Node::setForceX(double force) {
    forceX = force;
}

void Node::setForceY(double force) {
    forceY = force;
}

double Node::getForceX() const {
    return forceX;
}

double Node::getForceY() const {
    return forceY;
}