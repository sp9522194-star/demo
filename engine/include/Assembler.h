#pragma once

#include <Eigen/Dense>

#include "Model.h"

class Assembler {
public:

    static Eigen::MatrixXd
    assembleGlobalStiffness(const Model& model);
};