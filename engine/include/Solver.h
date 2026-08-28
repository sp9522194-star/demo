#pragma once

#include <Eigen/Dense>

class Solver {
public:

    static Eigen::VectorXd solve(
        const Eigen::MatrixXd& K,
        const Eigen::VectorXd& F
    );
};