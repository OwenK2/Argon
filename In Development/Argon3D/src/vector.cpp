#include "vector.h"

Vector::Vector() {
  dim = 0;
  r = 0;
}

Vector::Vector(uint8_t _dim, std::vector<double> _comps) {
  if(_comps.size() != dim) {
    dim = 0;

    comps = {};

    r = 0;
    angs = {};

    std::cout << "Vector error: dimension must match number of components passed." << std::endl;
  }
  else {
    dim = _dim;

    comps = _comps;

    std::vector<double> sphericals = sphericalFromCartesian(comps);
    r = sphericals[0];
    angs = std::vector<double>(sphericals.begin() + 1, sphericals.end());
  }
}
Vector::Vector(uint8_t _dim, double _r, std::vector<double> _angs) {
  if(_angs.size() != dim-1) {
    dim = 0;

    r = 0;
    angs = {};

    comps = {};
    std::cout << "Vector error: dimension must match number of components passed." << std::endl;
  }
  else {
    dim = _dim;

    r = _r;
    angs = _angs;

    comps = cartesianFromSpherical(angs, r);
  }
}

Vector::Vector(double _x, double _y) {
  dim = 2;

  comps = {_x, _y};

  std::vector<double> sphericals = sphericalFromCartesian(comps);
  r = sphericals[0];
  angs = std::vector<double>(sphericals.begin() + 1, sphericals.end());
}
Vector::Vector(double _r, double _theta, bool _spherical) {
  dim = 2;

  r = _r;
  angs = {_theta};

  comps = cartesianFromSpherical(angs, r);
}

Vector::Vector(double _x, double _y, double _z) {
  dim = 3;

  comps = {_x, _y, _z};

  std::vector<double> sphericals = sphericalFromCartesian(comps);
  r = sphericals[0];
  angs = std::vector<double>(sphericals.begin() + 1, sphericals.end());
}
Vector::Vector(double _r, double _theta, double _psi, bool _spherical) {
  dim = 3;

  r = _r;
  angs = {_theta, _psi};

  comps = cartesianFromSpherical(angs, r);
}

Vector::Vector(double _x, double _y, double _z, double _w) {
  dim = 4;

  comps = {_x, _y, _z, _w};

  std::vector<double> sphericals = sphericalFromCartesian(comps);
  r = sphericals[0];
  angs = std::vector<double>(sphericals.begin() + 1, sphericals.end());
}
Vector::Vector(double _r, double _theta, double _psi, double _phi, bool _spherical) {
  dim = 4;

  r = _r;
  angs = {_theta, _psi, _phi};

  comps = cartesianFromSpherical(angs, r);
}


Vector& Vector::scl(double scalar) {
  for(uint8_t d = 0; d < comps.size(); ++d) {
    comps[d] *= scalar;
  }
  r *= scalar;

  return *this;
}

Vector& Vector::add(Vector v) {
  if(dim != v.dim) {
    std::cout << "Can't add vectors of different dimensions" << std::endl;
    return *this;
  }
  for(uint8_t d = 0; d < dim; ++d) {
    comps[d] += v.comps[d];
  }
  return *this;
}
Vector& Vector::sub(Vector v) {
  if(dim != v.dim) {
    std::cout << "Can't subtract vectors of different dimensions" << std::endl;
    return *this;
  }
  for(uint8_t d = 0; d < dim; ++d) {
    comps[d] -= v.comps[d];
  }
  return *this;
}

double Vector::dot(Vector v) {
  if(dim != v.dim) {
    std::cout << "Can't dot vectors of different dimensions" << std::endl;
  }
  double dotProduct = 0;
  for(uint8_t d = 0; d < fmin(dim, v.dim); ++d) {
    dotProduct += comps[d] * v.comps[d];
  }
  return dotProduct;
}
Vector Vector::cross(Vector v) {
  Vector cp;
  return cp;
}

Vector& Vector::operator+=(Vector v) {
  return this->add(v);
}
Vector& Vector::operator-=(Vector v) {
  return this->sub(v);
}


std::vector<double> Vector::cartesianFromSpherical(std::vector<double> angles, double r = 1) {
  if(angles.empty()) {
    return std::vector<double>{r};
  }

  std::vector<double> components(angles.size() + 1); // [x, y, z, w, ...]

  components[0] = r*cos(angles[0]);
  components[1] = r*sin(angles[0]);
  for(uint8_t d = 2; d < components.size(); ++d) {
    components[d] = r*cos(angles[d-1]);
  }

  for(uint8_t d = components.size() - 1; d >= 0; --d) {
    for(uint8_t a = d; a < angles.size(); ++a) {
      if(d == 0 && a == 0) {
        continue;
      }
      components[d] *= sin(angles[a]);
    }
  }

  return components;
}
std::vector<double> Vector::sphericalFromCartesian(std::vector<double> components) {
  if(components.empty()) {
    return std::vector<double>{0.0};
  }

  std::vector<double> sphericals(components.size()); // [r, a1, a2, a2, ...]

  double normSquared = 0;
  for(uint8_t d = 0; d < components.size(); ++d) {
    if(d == 1) {
      sphericals[1] = atan2(components[1], components[0]);
    }
    if(d > 1) {
      sphericals[d] = atan2(sqrt(normSquared), components[d]);
    }
    normSquared += pow(components[d], 2);
  }
  sphericals[0] = sqrt(normSquared);

  return sphericals;
}
