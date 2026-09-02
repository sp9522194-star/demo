#include "Assembler.h"

#include <stdexcept>

Eigen::MatrixXd
Assembler::assembleGlobalStiffness(
    const Model& model
) {

    const auto& nodes = model.getNodes();
    const auto& elements = model.getElements();

    int numberOfDOFs =
        2 * static_cast<int>(nodes.size());

    Eigen::MatrixXd K =
        Eigen::MatrixXd::Zero(
            numberOfDOFs,
            numberOfDOFs
        );

    for (const TrussElement& element : elements) {

        Eigen::Matrix4d ke =
            element.getStiffnessMatrix();

        int node1Id =
            element.getNode1Id();

        int node2Id =
            element.getNode2Id();

        int node1Index = -1;
        int node2Index = -1;

        for (int i = 0;
             i < static_cast<int>(nodes.size());
             ++i) {

            if (nodes[i].getId() == node1Id) {
                node1Index = i;
            }

            if (nodes[i].getId() == node2Id) {
                node2Index = i;
            }
        }

        if (node1Index == -1 ||
            node2Index == -1) {

            throw std::runtime_error(
                "Element references a node "
                "that does not exist in the model."
            );
        }

        int dofs[4] = {
            2 * node1Index,
            2 * node1Index + 1,
            2 * node2Index,
            2 * node2Index + 1
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