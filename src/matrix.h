#ifndef MATRIX_H
#define MATRIX_H


class Matrix {
  public:
    uint8_t rows, cols;
    std::vector<double> m;

    Matrix();
    ~Matrix();
}


#endif
