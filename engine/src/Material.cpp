#include "Material.h"

Material::Material(double youngsModulus, double area)
    : youngsModulus(youngsModulus),
      area(area) {
}

double Material::getYoungsModulus() const {
    return youngsModulus;
}

double Material::getArea() const {
    return area;
}