#include "Solver.h"

#include <stdexcept>

Eigen::VectorXd Solver::solve(
    const Eigen::MatrixXd& K,
    const Eigen::VectorXd& F
) {

    if (K.rows() != K.cols()) {
        throw std::runtime_error(
            "Stiffness matrix K must be square."
        );
    }

    if (K.rows() != F.size()) {
        throw std::runtime_error(
            "K and F dimensions do not match."
        );
    }

    Eigen::FullPivLU<Eigen::MatrixXd> lu(K);

    if (!lu.isInvertible()) {
        throw std::runtime_error(
            "FEA system is singular or unstable."
        );
    }

    Eigen::VectorXd U = lu.solve(F);

    return U;
}