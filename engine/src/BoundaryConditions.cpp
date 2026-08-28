#include "BoundaryConditions.h"

void BoundaryConditions::apply(
    Eigen::MatrixXd& K,
    Eigen::VectorXd& F,
    const Model& model
) {

    const auto& nodes = model.getNodes();

    for (const Node& node : nodes) {

        int baseDOF =
            2 * (node.getId() - 1);

        int dofs[2] = {
            baseDOF,
            baseDOF + 1
        };

        bool fixed[2] = {
            node.isFixedX(),
            node.isFixedY()
        };

        for (int i = 0; i < 2; ++i) {

            if (!fixed[i]) {
                continue;
            }

            int dof = dofs[i];

            K.row(dof).setZero();
            K.col(dof).setZero();

            K(dof, dof) = 1.0;

            F(dof) = 0.0;
        }
    }
}