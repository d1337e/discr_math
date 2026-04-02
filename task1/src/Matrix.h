#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <limits>
#include <random>


enum FILLING {
    MANUAL = 1,
    RANDOM = 2,
};


enum ACTIONS {
    ADDITION = 1,
    SUBSTRACTION = 2,
    MULTIPLICATION = 3,
    MULTIPLICATION_SCALAR = 4,
    DETERMINATE = 5,
    RANK = 6,
    INVERSEFORM = 7,
    INVERSEGAY = 8,
    EXIT = 0,
};

class Matrix {
private:
    std::vector<std::vector<double>> matrix;
    int rows, cols;
public:
    Matrix(const int n, const int m);
    Matrix(const int n, const int m, const double value);
    ~Matrix() = default;
    void fill_from_user();
    void fill_random(const double min = -10., const double max = 10.);
    void transposition();
    void inverse_by_form();
    void inverse_by_regular();
    Matrix& operator=(const Matrix& other);
    friend Matrix operator+(const Matrix& a, const Matrix& b);
    friend Matrix operator-(const Matrix& a, const Matrix& b);
    friend Matrix operator*(const Matrix& a, const Matrix& b);
    friend Matrix operator*(const Matrix& a, const double value);
    void print_matrix() const;
    double det() const;
    int rank() const;
    Matrix minor(const int row_del, const int col_del) const;
    void run_matrix();
    int get_rows() const;
    int get_cols() const;
    double& operator()(const int i, const int j);
    double operator()(const int i, const int j) const;
};