#include <iostream>
#include <vector>
#include <cmath>

#include <Eigen/Dense>

#include "Node.h"
#include "Material.h"
#include "TrussElement.h"
#include "Model.h"
#include "Assembler.h"
#include "LoadVector.h"
#include "BoundaryConditions.h"
#include "Solver.h"
#include "StressAnalysis.h"
#include "Validation.h"
#include "TrussMeshGenerator.h"

int main() {

    std::cout << "========================================\n";
    std::cout << "           2D FEA ENGINE\n";
    std::cout << "========================================\n\n";


    // ============================================================
    // 1. MATERIAL
    // ============================================================

    Material steel(
        200e9,       // Young's modulus = 200 GPa
        0.01         // Area = 0.01 m^2
    );

    std::cout << "Material created.\n";

    std::cout << "Young's Modulus = "
              << steel.getYoungsModulus()
              << " Pa\n";

    std::cout << "Area = "
              << steel.getArea()
              << " m^2\n";


    // ============================================================
    // 2. MESH GENERATION TEST
    // ============================================================

    std::cout
        << "\n========== MESH TEST ==========\n\n";

    Node start(
        1,
        0.0,
        0.0
    );

    Node end(
        2,
        5.0,
        0.0
    );

    int divisions = 4;

    std::vector<Node> meshNodes =
        TrussMeshGenerator::generateNodes(
            start,
            end,
            divisions,
            10
        );

    for (const Node& node : meshNodes) {

        std::cout
            << "Node "
            << node.getId()
            << ": ("
            << node.getX()
            << ", "
            << node.getY()
            << ")\n";
    }


    // ============================================================
    // 3. ELEMENT MESH GENERATION
    // ============================================================

    std::vector<TrussElement> meshElements =
        TrussMeshGenerator::generateElements(
            meshNodes,
            steel,
            1
        );

    std::cout
        << "\n========== ELEMENT MESH ==========\n\n";

    for (const TrussElement& element :
         meshElements) {

        std::cout
            << "Element "
            << element.getId()
            << ": Node "
            << element.getNode1Id()
            << " -> Node "
            << element.getNode2Id()
            << "\n";
    }


    // ============================================================
    // 4. ORIGINAL 3-NODE TRUSS MODEL
    // ============================================================

    std::cout
        << "\n========== ORIGINAL FEA MODEL ==========\n\n";

    Model model;


    Node node1(
        1,
        0.0,
        0.0
    );

    Node node2(
        2,
        5.0,
        0.0
    );

    Node node3(
        3,
        2.5,
        5.0
    );


    // ============================================================
    // 5. BOUNDARY CONDITIONS
    // ============================================================

    // Node 1 fixed in X and Y
    node1.setFixedX(true);
    node1.setFixedY(true);

    // Node 2 fixed in Y
    node2.setFixedY(true);


    // ============================================================
    // 6. LOAD
    // ============================================================

    // Downward load at Node 3
    node3.setForceY(
        -10000.0
    );


    // ============================================================
    // 7. ADD NODES
    // ============================================================

    model.addNode(node1);
    model.addNode(node2);
    model.addNode(node3);


    // ============================================================
    // 8. CREATE ELEMENTS
    // ============================================================

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

    model.addElement(element1);
    model.addElement(element2);
    model.addElement(element3);


    // ============================================================
    // 9. ELEMENT STIFFNESS MATRICES
    // ============================================================

    std::cout
        << "\n========== ELEMENT STIFFNESS MATRICES ==========\n";

    for (const TrussElement& element :
         model.getElements()) {

        std::cout
            << "\nElement "
            << element.getId()
            << ":\n\n";

        std::cout
            << element.getStiffnessMatrix()
            << "\n";
    }


    // ============================================================
    // 10. GLOBAL STIFFNESS MATRIX
    // ============================================================

    std::cout
        << "\n========== GLOBAL STIFFNESS MATRIX ==========\n";

    Eigen::MatrixXd K =
        Assembler::assembleGlobalStiffness(
            model
        );

    std::cout
        << "\nK =\n"
        << K
        << "\n";


    // ============================================================
    // 11. GLOBAL LOAD VECTOR
    // ============================================================

    Eigen::VectorXd F =
        Eigen::VectorXd::Zero(
            2 * model.getNodes().size()
        );

    for (int i = 0;
         i < static_cast<int>(
                 model.getNodes().size());
         ++i) {

        const Node& node =
            model.getNodes()[i];

        F(2 * i) =
            node.getForceX();

        F(2 * i + 1) =
            node.getForceY();
    }

    std::cout
        << "\n========== LOAD VECTOR ==========\n\n";

    std::cout
        << "F =\n"
        << F
        << "\n";


    // ============================================================
    // 12. APPLY BOUNDARY CONDITIONS
    // ============================================================

    BoundaryConditions::apply(
        K,
        F,
        model
    );

    std::cout
        << "\n========== MODIFIED SYSTEM ==========\n";

    std::cout
        << "\nModified K =\n"
        << K
        << "\n";

    std::cout
        << "\nModified F =\n"
        << F
        << "\n";


    // ============================================================
    // 13. SOLVE KU = F
    // ============================================================

    Eigen::VectorXd U =
        Solver::solve(
            K,
            F
        );

    std::cout
        << "\n========== DISPLACEMENTS ==========\n\n";

    for (int i = 0;
         i < U.size();
         i += 2) {

        std::cout
            << "Node "
            << model
                   .getNodes()[i / 2]
                   .getId()
            << ": Ux = "
            << U(i) * 1000.0
            << " mm, Uy = "
            << U(i + 1) * 1000.0
            << " mm\n";
    }


    // ============================================================
    // 14. STRESS / STRAIN ANALYSIS
    // ============================================================

    std::cout
        << "\n========== ELEMENT RESULTS ==========\n\n";

    std::vector<ElementResult> results =
        StressAnalysis::analyze(
            model,
            U
        );

    for (const ElementResult& result :
         results) {

        std::cout
            << "Element "
            << result.elementId
            << ":\n";

        std::cout
            << "  Strain      = "
            << result.strain
            << "\n";

        std::cout
            << "  Stress      = "
            << result.stress / 1e6
            << " MPa\n";

        std::cout
            << "  Axial Force = "
            << result.axialForce / 1000.0
            << " kN\n\n";
    }


    // ============================================================
    // 15. ANALYTICAL VALIDATION
    // ============================================================

    std::cout
        << "\n========== ANALYTICAL VALIDATION ==========\n\n";

    Validation::run(
        model,
        U
    );


    // ============================================================
    // 16. REFINED BAR MESH
    // ============================================================

    std::cout
        << "\n========== REFINED BAR MESH ==========\n\n";

    Node barStart(
        1,
        0.0,
        0.0
    );

    Node barEnd(
        5,
        5.0,
        0.0
    );

    int barDivisions = 4;


    // Generate 5 nodes
    std::vector<Node> refinedNodes =
        TrussMeshGenerator::generateNodes(
            barStart,
            barEnd,
            barDivisions,
            1
        );


    // Generate 4 elements
    std::vector<TrussElement> refinedElements =
        TrussMeshGenerator::generateElements(
            refinedNodes,
            steel,
            1
        );


    // ============================================================
    // 17. PRINT REFINED NODES
    // ============================================================

    std::cout
        << "Nodes:\n\n";

    for (const Node& node :
         refinedNodes) {

        std::cout
            << "Node "
            << node.getId()
            << ": ("
            << node.getX()
            << ", "
            << node.getY()
            << ")\n";
    }


    // ============================================================
    // 18. PRINT REFINED ELEMENTS
    // ============================================================

    std::cout
        << "\nElements:\n\n";

    for (const TrussElement& element :
         refinedElements) {

        std::cout
            << "Element "
            << element.getId()
            << ": Node "
            << element.getNode1Id()
            << " -> Node "
            << element.getNode2Id()
            << "\n";
    }


    // ============================================================
    // 19. REFINED BAR FEA
    // ============================================================

    std::cout
        << "\n========== REFINED BAR FEA ==========\n\n";

    Model refinedModel;


    // ------------------------------------------------------------
    // STEP A
    // ------------------------------------------------------------

    std::cout
        << "Step A: Creating refined model...\n";


    // ------------------------------------------------------------
    // STEP B
    // Add nodes + boundary conditions + load
    // ------------------------------------------------------------

    for (Node& node :
         refinedNodes) {

        /*
            This is a horizontal axial bar.

            Therefore vertical displacement is constrained
            at every node.

            This removes unnecessary free Y DOFs and makes
            the axial-bar verification problem stable.
        */

        node.setFixedY(true);


        // Left end fixed in X
        if (node.getId() == 1) {

            node.setFixedX(true);
        }


        // 10,000 N tensile load at right end
        if (node.getId() == 5) {

            node.setForceX(
                10000.0
            );
        }


        refinedModel.addNode(
            node
        );
    }

    std::cout
        << "Step B: Nodes added to model.\n";


    // ------------------------------------------------------------
    // STEP C
    // Add elements
    // ------------------------------------------------------------

    for (const TrussElement& element :
         refinedElements) {

        refinedModel.addElement(
            element
        );
    }

    std::cout
        << "Step C: Elements added to model.\n";


    // ============================================================
    // 20. ASSEMBLE REFINED GLOBAL STIFFNESS MATRIX
    // ============================================================

    std::cout
        << "Step D: Assembling stiffness matrix...\n";

    Eigen::MatrixXd refinedK =
        Assembler::assembleGlobalStiffness(
            refinedModel
        );

    std::cout
        << "Step E: Stiffness matrix assembled.\n";


    // ============================================================
    // 21. CREATE REFINED LOAD VECTOR
    // ============================================================

    Eigen::VectorXd refinedF =
        Eigen::VectorXd::Zero(
            2 * refinedModel.getNodes().size()
        );

    for (int i = 0;
         i < static_cast<int>(
                 refinedModel.getNodes().size());
         ++i) {

        const Node& node =
            refinedModel.getNodes()[i];

        refinedF(2 * i) =
            node.getForceX();

        refinedF(2 * i + 1) =
            node.getForceY();
    }


    // ============================================================
    // 22. APPLY REFINED BOUNDARY CONDITIONS
    // ============================================================

    std::cout
        << "Step F: Applying boundary conditions...\n";

    BoundaryConditions::apply(
        refinedK,
        refinedF,
        refinedModel
    );

    std::cout
        << "Step G: Boundary conditions applied.\n";


    // ============================================================
    // 23. SOLVE REFINED SYSTEM
    // ============================================================

    std::cout
        << "Step H: Solving system...\n";

    Eigen::VectorXd refinedU =
        Solver::solve(
            refinedK,
            refinedF
        );

    std::cout
        << "Step I: System solved.\n";


    // ============================================================
    // 24. PRINT REFINED DISPLACEMENTS
    // ============================================================

    std::cout
        << "\n========== REFINED BAR DISPLACEMENTS ==========\n\n";

    for (int i = 0;
         i < refinedU.size();
         i += 2) {

        std::cout
            << "Node "
            << refinedModel
                   .getNodes()[i / 2]
                   .getId()
            << ": Ux = "
            << refinedU(i) * 1000.0
            << " mm, Uy = "
            << refinedU(i + 1) * 1000.0
            << " mm\n";
    }


    // ============================================================
    // 25. ANALYTICAL BAR SOLUTION
    // ============================================================

    /*
                P L
        u = -----------
              A E

        P = 10000 N
        L = 5 m
        A = 0.01 m^2
        E = 200e9 Pa

        u = 0.000025 m
          = 0.025 mm
    */

    double analyticalBarDisplacement =
        (
            10000.0 * 5.0
        )
        /
        (
            0.01 * 200e9
        );


    // Node 5 X displacement
    // Node 5 = index 4
    // X DOF = 2 * 4 = 8

    double feaBarDisplacement =
        refinedU(8);


    // ============================================================
    // 26. ERROR
    // ============================================================

    double barError =
        std::abs(
            feaBarDisplacement
            -
            analyticalBarDisplacement
        )
        /
        std::abs(
            analyticalBarDisplacement
        )
        * 100.0;


    // ============================================================
    // 27. REFINED VALIDATION RESULT
    // ============================================================

    std::cout
        << "\n========== REFINED MESH VALIDATION ==========\n\n";

    std::cout
        << "Analytical displacement = "
        << analyticalBarDisplacement * 1000.0
        << " mm\n";

    std::cout
        << "FEA displacement        = "
        << feaBarDisplacement * 1000.0
        << " mm\n";

    std::cout
        << "Error                   = "
        << barError
        << " %\n";


    if (barError < 1e-6) {

        std::cout
            << "\nREFINED MESH VALIDATION PASSED\n";
    }
    else {

        std::cout
            << "\nREFINED MESH VALIDATION FAILED\n";
    }


    // ============================================================
    // 28. COMPLETE
    // ============================================================

    std::cout
        << "\n========================================\n";

    std::cout
        << "       FEA ANALYSIS COMPLETE\n";

    std::cout
        << "========================================\n";


    return 0;
}