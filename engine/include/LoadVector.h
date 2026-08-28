#pragma once

#include <Eigen/Dense>

#include "Model.h"

class LoadVector {
public:

    static Eigen::VectorXd
    create(const Model& model);
};