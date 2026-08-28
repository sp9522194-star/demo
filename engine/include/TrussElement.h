#pragma once

#include <Eigen/Dense>

#include "Node.h"
#include "Material.h"

class TrussElement {

public:

    TrussElement(
        int id,
        const Node& node1,
        const Node& node2,
        const Material& material
    );

    int getId() const;

    double getLength() const;

    int getNode1Id() const;

    int getNode2Id() const;

    const Node& getNode1() const;

    const Node& getNode2() const;

    const Material& getMaterial() const;

    Eigen::Matrix4d getStiffnessMatrix() const;

private:

    int id;

    Node node1;

    Node node2;

    Material material;
};