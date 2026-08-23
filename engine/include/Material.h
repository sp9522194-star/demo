#pragma once 
class Material{
    public:
        Material(double youngsModulus,double area);

        double getYoungsModulus() const;
        double getArea() const;
    
    private:
        double youngsModulus;
        double area;
};