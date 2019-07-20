#include "matrix.h"

Matrix::Matrix() {
  rows = 0;
  cols = 0;
  m = {};
}

Matrix::Matrix(uint8_t _rows, uint8_t _cols) : rows(_rows), cols(_cols) {
  m = std::vector<double>(rows*cols);
}

Matrix::Matrix(uint8_t _rows, uint8_t _cols, std::vector<double> _m) : rows(_rows), cols(_cols), m(_m) {}


Matrix* Matrix::transpose() {
  std::vector<double> mT;
  for(uint8_t c = 0; c < cols; ++c) {
    for(uint8_t r = 0; r < rows; ++r) {
      mT[c*rows+r] = m[r*cols+c];
    }
  }

  m = mT;

  uint8_t temp = rows;
  rows = cols;
  cols = temp;

  return this;
}

Matrix* Matrix::scale(double scalar) {
  for(uint16_t a = 0; a < m.size(); ++a) {
    m[a] *= scalar;
  }
  return this;
}

Matrix* Matrix::compose(Matrix _m) {
  if(cols != _m.rows) {
    std::cout << "Cannot compose an " << rows << "x" << cols << "matrix with a " << _m.rows << "x" << _m.cols << "matrix." << std::endl;
    return this;
  }

  std::vector<double> composed(rows*_m.cols);
  for(uint8_t r = 0; r < rows; ++r) {
    for(uint8_t c = 0; c < _m.cols; ++c) {
      for(uint8_t i = 0; i < cols; ++i) {
        composed[r*_m.cols+c] += m[r*cols+i]*_m.m[i*_m.cols+c];
      }
    }
  }
  m = composed;
  cols = _m.cols;
  return this;
}
