#include "Validation.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>

void Validation::run(
    const Model& model,
    const Eigen::VectorXd& U
) {
    const auto& nodes = model.getNodes();
    const auto& elements = model.getElements();

    if (nodes.size() < 3 || elements.size() < 3) {
        throw std::runtime_error(
            "Validation requires the 3-node truss model."
        );
    }

    double E =
        elements[0].getMaterial().getYoungsModulus();

    double A =
        elements[0].getMaterial().getArea();

    // Applied load
    double P =
        std::abs(nodes[2].getForceY());

    // ------------------------------------------------
    // Actual member forces from structural equilibrium
    // ------------------------------------------------

    double N1 = P / 4.0;

    double N2 =
        -P / (2.0 * (5.0 / std::sqrt(31.25)));

    double N3 = N2;

    // ------------------------------------------------
    // Element lengths
    // ------------------------------------------------

    double L1 = elements[0].getLength();
    double L2 = elements[1].getLength();
    double L3 = elements[2].getLength();

    // ------------------------------------------------
    // Unit-load member forces
    // Unit vertical load applied at Node 3
    // ------------------------------------------------

    double n1 = 0.25;

    double n2 =
        -1.0 /
        (2.0 * (5.0 / std::sqrt(31.25)));

    double n3 = n2;

    // ------------------------------------------------
    // Unit-load / virtual-work displacement
    //
    // delta = sum(N_i * n_i * L_i / A E)
    // ------------------------------------------------

    double analyticalUy =
        (
            N1 * n1 * L1
            +
            N2 * n2 * L2
            +
            N3 * n3 * L3
        ) / (A * E);

    // FEA displacement at Node 3, Y direction
    int node3DOF =
        2 * (nodes[2].getId() - 1);

    double feaUy =
        U(node3DOF + 1);

    // Analytical displacement is magnitude.
    // Apply downward sign because load is downward.
    analyticalUy = -analyticalUy;

    double error =
        std::abs(feaUy - analyticalUy)
        /
        std::abs(analyticalUy)
        * 100.0;

    std::cout
        << "\n========== ANALYTICAL VALIDATION ==========\n\n";

    std::cout
        << std::fixed
        << std::setprecision(9);

    std::cout
        << "Analytical member forces:\n";

    std::cout
        << "  Element 1 = "
        << N1 / 1000.0
        << " kN\n";

    std::cout
        << "  Element 2 = "
        << N2 / 1000.0
        << " kN\n";

    std::cout
        << "  Element 3 = "
        << N3 / 1000.0
        << " kN\n\n";

    std::cout
        << "Node 3 vertical displacement:\n";

    std::cout
        << "  FEA        = "
        << feaUy * 1000.0
        << " mm\n";

    std::cout
        << "  Analytical = "
        << analyticalUy * 1000.0
        << " mm\n";

    std::cout
        << "  Error      = "
        << error
        << " %\n\n";

    if (error < 1e-6) {
        std::cout
            << "VALIDATION PASSED\n";
    }
    else {
        std::cout
            << "VALIDATION FAILED\n";
    }

    std::cout
        << "\n============================================\n";
}