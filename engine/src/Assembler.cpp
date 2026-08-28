#include "Assembler.h"

#include <stdexcept>

Eigen::MatrixXd
Assembler::assembleGlobalStiffness(const Model& model) {

    const auto& nodes = model.getNodes();
    const auto& elements = model.getElements();

    int numberOfDOFs = 2 * nodes.size();

    Eigen::MatrixXd K =
        Eigen::MatrixXd::Zero(
            numberOfDOFs,
            numberOfDOFs
        );

    for (const TrussElement& element : elements) {

        Eigen::Matrix4d ke =
            element.getStiffnessMatrix();

        int node1 = element.getNode1Id();
        int node2 = element.getNode2Id();

        int dofs[4] = {
            2 * (node1 - 1),
            2 * (node1 - 1) + 1,
            2 * (node2 - 1),
            2 * (node2 - 1) + 1
        };

        for (int i = 0; i < 4; ++i) {

            for (int j = 0; j < 4; ++j) {

                K(dofs[i], dofs[j])
                    += ke(i, j);
            }
        }
    }

    return K;
}