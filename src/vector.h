#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <vector>
#include <cmath>

class Vector {
  public:
    uint8_t dim;

    std::vector<double> comps; // [x, y, z, w, ...]

    double r;
    std::vector<double> angs; // [xy, z, w, ...]

    //Rn
    Vector(uint8_t _dim, std::vector<double> _comps);
    Vector(uint8_t _dim, double _r, std::vector<double> _angs);
    //R2
    Vector(double x, double y);
    Vector(double r, double theta, bool _spherical);
    //R3
    Vector(double x, double y, double z);
    Vector(double r, double theta, double psi, bool _spherical);
    //R4
    Vector(double x, double y, double z, double w);
    Vector(double r, double theta, double psi, double phi, bool _spherical);

    ~Vector();

    void scale(double scalar);

    static std::vector<double> cartesianFromSpherical(std::vector<double> angles, double r);
    static std::vector<double> sphericalFromCartesian(std::vector<double> components);
};

#endif
