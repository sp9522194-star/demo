#include "LoadVector.h"

Eigen::VectorXd
LoadVector::create(const Model& model) {

    const auto& nodes = model.getNodes();

    int numberOfDOFs =
        2 * static_cast<int>(nodes.size());

    Eigen::VectorXd F =
        Eigen::VectorXd::Zero(numberOfDOFs);

    for (const Node& node : nodes) {

        int baseDOF =
            2 * (node.getId() - 1);

        F(baseDOF) =
            node.getForceX();

        F(baseDOF + 1) =
            node.getForceY();
    }

    return F;
}