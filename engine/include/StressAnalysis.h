#pragma once

#include <vector>

#include <Eigen/Dense>

#include "Model.h"

struct ElementResult {

    int elementId;

    double strain;

    double stress;

    double axialForce;
};

class StressAnalysis {

public:

    static std::vector<ElementResult> analyze(
        const Model& model,
        const Eigen::VectorXd& U
    );
};