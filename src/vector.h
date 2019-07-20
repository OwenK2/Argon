#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <vector>
#include <cmath>

class Vector {
  public:
    uint8_t dim;
    double* x, y, z, w;

    std::vector<double> comps; // [x, y, z, w, ...]

    double r;
    std::vector<double> angs; // [xy, z, w, ...]

    Vector();
    //Rn
    Vector(uint8_t _dim, std::vector<double> _comps);
    Vector(uint8_t _dim, double _r, std::vector<double> _angs);
    //R2
    Vector(double _x, double _y);
    Vector(double _r, double _theta, bool _spherical);
    //R3
    Vector(double _x, double _y, double _z);
    Vector(double _r, double _theta, double _psi, bool _spherical);
    //R4
    Vector(double _x, double _y, double _z, double _w);
    Vector(double _r, double _theta, double _psi, double _phi, bool _spherical);

    virtual ~Vector() {};

    Vector* scl(double scalar);
    Vector* add(Vector v);
    Vector* sub(Vector v);

    double dot(Vector v);
    Vector* cross(Vector v);

    Vector* operator+(Vector v);
    Vector* operator-(Vector v);

    static std::vector<double> cartesianFromSpherical(std::vector<double> angles, double r);
    static std::vector<double> sphericalFromCartesian(std::vector<double> components);
};

#endif
