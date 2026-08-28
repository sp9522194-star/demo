#pragma once

#include <Eigen/Dense>

#include "Model.h"

class BoundaryConditions {
public:

    static void apply(
        Eigen::MatrixXd& K,
        Eigen::VectorXd& F,
        const Model& model
    );
};