#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>

class Matrix {
  public:
    uint8_t rows;
    uint8_t cols;

    std::vector<double> m;

    Matrix();

    Matrix(uint8_t _rows, uint8_t _cols);
    Matrix(uint8_t _rows, uint8_t _cols, std::vector<double> _m);

    virtual ~Matrix();

    Matrix* transpose();

    Matrix* scale(double scalar);

    Matrix* compose(Matrix _m);
};


#endif
