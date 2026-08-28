#include "StressAnalysis.h"

#include <cmath>
#include <stdexcept>
#include"TrussElement.h"

std::vector<ElementResult>
StressAnalysis::analyze(
    const Model& model,
    const Eigen::VectorXd& U
) {

    std::vector<ElementResult> results;

    for (const TrussElement& element :
         model.getElements()) {

        // ====================================
        // Get element nodes
        // ====================================

        const Node& node1 =
            element.getNode1();

        const Node& node2 =
            element.getNode2();

        // ====================================
        // Element geometry
        // ====================================

        double dx =
            node2.getX() - node1.getX();

        double dy =
            node2.getY() - node1.getY();

        double L =
            std::sqrt(
                dx * dx +
                dy * dy
            );

        if (L <= 0.0) {

            throw std::runtime_error(
                "Element has zero length."
            );
        }

        // ====================================
        // Direction cosines
        // ====================================

        double c =
            dx / L;

        double s =
            dy / L;

        // ====================================
        // Global DOF indices
        // ====================================

        int i =
            2 * (node1.getId() - 1);

        int j =
            2 * (node2.getId() - 1);

        // ====================================
        // Nodal displacements
        // ====================================

        double uix = U(i);
        double uiy = U(i + 1);

        double ujx = U(j);
        double ujy = U(j + 1);

        // ====================================
        // Change in element length
        // ====================================

        double deltaL =
            -c * uix
            -s * uiy
            +c * ujx
            +s * ujy;

        // ====================================
        // Strain
        // ====================================

        double strain =
            deltaL / L;

        // ====================================
        // Stress
        // ====================================

        double E =
            element
                .getMaterial()
                .getYoungsModulus();

        double stress =
            E * strain;

        // ====================================
        // Axial force
        // ====================================

        double A =
            element
                .getMaterial()
                .getArea();

        double axialForce =
            stress * A;

        // ====================================
        // Store result
        // ====================================

        ElementResult result;

        result.elementId =
            element.getId();

        result.strain =
            strain;

        result.stress =
            stress;

        result.axialForce =
            axialForce;

        results.push_back(result);
    }

    return results;
}