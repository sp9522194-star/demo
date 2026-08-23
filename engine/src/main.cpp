#include <iostream>

#include "Node.h"
#include "Material.h"
#include "Model.h"

int main() {

    Model model;

    Node node1(1, 0.0, 0.0);
    Node node2(2, 5.0, 0.0);

    model.addNode(node1);
    model.addNode(node2);

    Material steel(
        200e9,   // Young's modulus: 200 GPa
        0.01     // Area: 0.01 m²
    );

    std::cout << "FEA Engine initialized successfully.\n\n";

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

    std::cout << "\nMaterial:\n";

    std::cout
        << "Young's Modulus = "
        << steel.getYoungsModulus()
        << " Pa\n";

    std::cout
        << "Area = "
        << steel.getArea()
        << " m^2\n";

    return 0;
}