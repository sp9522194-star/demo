#pragma once

#include "Model.h"
#include <Eigen/Dense>

class Validation {
public:
    static void run(
        const Model& model,
        const Eigen::VectorXd& U
    );
};