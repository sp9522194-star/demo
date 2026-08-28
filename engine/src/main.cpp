#include <cmath>
#include <iomanip>
#include <iostream>

#include "Node.h"
#include "Material.h"
#include "Model.h"
#include "TrussElement.h"
#include "Assembler.h"
#include "LoadVector.h"
#include "BoundaryConditions.h"
#include "Solver.h"

int main() {

    // ========================================
    // 1. Create model
    // ========================================

    Model model;

    // ========================================
    // 2. Create nodes
    // ========================================

    Node node1(1, 0.0, 0.0);
    Node node2(2, 5.0, 0.0);
    Node node3(3, 2.5, 5.0);

    // ========================================
    // 3. Apply boundary conditions
    // ========================================

    // Node 1: fixed in X and Y
    node1.setFixedX(true);
    node1.setFixedY(true);

    // Node 2: roller support
    // Fixed only in Y
    node2.setFixedY(true);

    // ========================================
    // 4. Apply external load
    // ========================================

    // 10 kN downward force at Node 3
    node3.setForceY(-10000.0);

    // ========================================
    // 5. Add nodes to model
    // ========================================

    model.addNode(node1);
    model.addNode(node2);
    model.addNode(node3);

    // ========================================
    // 6. Material
    // ========================================

    // Steel
    // Young's modulus = 200 GPa
    // Area = 0.01 m^2

    Material steel(
        200e9,
        0.01
    );

    // ========================================
    // 7. Create elements
    // ========================================

    TrussElement element1(
        1,
        node1,
        node2,
        steel
    );

    TrussElement element2(
        2,
        node2,
        node3,
        steel
    );

    TrussElement element3(
        3,
        node1,
        node3,
        steel
    );

    // ========================================
    // 8. Add elements to model
    // ========================================

    model.addElement(element1);
    model.addElement(element2);
    model.addElement(element3);

    // ========================================
    // 9. Display model
    // ========================================

    std::cout << std::fixed
              << std::setprecision(6);

    std::cout
        << "========================================\n"
        << "        2D STRUCTURAL FEA ENGINE       \n"
        << "========================================\n\n";

    std::cout << "Nodes:\n";

    for (const Node& node : model.getNodes()) {

        std::cout
            << "Node "
            << node.getId()
            << ": ("
            << node.getX()
            << ", "
            << node.getY()
            << ")\n";
    }

    std::cout << "\nElements:\n";

    for (const TrussElement& element :
         model.getElements()) {

        std::cout
            << "Element "
            << element.getId()
            << ": Node "
            << element.getNode1Id()
            << " -> Node "
            << element.getNode2Id()
            << ", Length = "
            << element.getLength()
            << " m\n";
    }

    // ========================================
    // 10. Assemble global stiffness matrix K
    // ========================================

    Eigen::MatrixXd K =
        Assembler::assembleGlobalStiffness(model);

    std::cout
        << "\nGlobal stiffness matrix K:\n\n";

    std::cout << K << "\n";

    // ========================================
    // 11. Create global force vector F
    // ========================================

    Eigen::VectorXd F =
        LoadVector::create(model);

    std::cout
        << "\nForce vector F before boundary conditions:\n\n";

    std::cout << F << "\n";

    // ========================================
    // 12. Apply boundary conditions
    // ========================================

    BoundaryConditions::apply(
        K,
        F,
        model
    );

    // ========================================
    // 13. Display modified system
    // ========================================

    std::cout
        << "\nK after boundary conditions:\n\n";

    std::cout << K << "\n";

    std::cout
        << "\nF after boundary conditions:\n\n";

    std::cout << F << "\n";

    // ========================================
    // 14. Check matrix properties
    // ========================================

    std::cout
        << "\nMatrix size: "
        << K.rows()
        << " x "
        << K.cols()
        << "\n";

    std::cout
        << "Is K symmetric? "
        << K.isApprox(K.transpose())
        << "\n";

    // ========================================
    // 15. Solve KU = F
    // ========================================

    try {

        Eigen::VectorXd U =
            Solver::solve(K, F);

        // ====================================
        // 16. Print displacement vector
        // ====================================

        std::cout
            << "\nDisplacement vector U (meters):\n\n";

        std::cout << U << "\n";

        // ====================================
        // 17. Display nodal displacements
        // ====================================

        std::cout
            << "\n========== FEA SOLUTION ==========\n\n";

        for (const Node& node : model.getNodes()) {

            int baseDOF =
                2 * (node.getId() - 1);

            double ux =
                U(baseDOF);

            double uy =
                U(baseDOF + 1);

            // Convert meters → millimeters
            double uxMM =
                ux * 1000.0;

            double uyMM =
                uy * 1000.0;

            std::cout
                << "Node "
                << node.getId()
                << ":\n";

            std::cout
                << "  Ux = "
                << uxMM
                << " mm\n";

            std::cout
                << "  Uy = "
                << uyMM
                << " mm\n\n";
        }

        // ====================================
        // 18. Calculate maximum displacement
        // ====================================

        double maxDisplacement = 0.0;

        for (int i = 0; i < U.size(); ++i) {

            double displacement =
                std::abs(U(i));

            if (displacement > maxDisplacement) {
                maxDisplacement = displacement;
            }
        }

        std::cout
            << "Maximum displacement = "
            << maxDisplacement * 1000.0
            << " mm\n";

        std::cout
            << "\n========================================\n";

    }
    catch (const std::exception& error) {

        std::cerr
            << "\n========== FEA ERROR ==========\n\n";

        std::cerr
            << error.what()
            << "\n";

        std::cerr
            << "\n================================\n";

        return 1;
    }

    return 0;
}