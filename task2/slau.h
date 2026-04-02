#pragma once
#include "../task1/src/Matrix.h"
#include <limits>
#include <iostream>

enum SLAU_STATUS {
    NOT_SOLVED,
    UNIQUE_SOLUTION,
    NO_SOLUTION,
    INFINITE_SOLUTIONS,
    CRAMER_NEMA,
};

enum MODE {
    CRAMER = 1,
    GAUSS,
};

class SLAU {
private:
    Matrix a;
    Matrix b;
    Matrix x;
    SLAU_STATUS solve;
public:
    SLAU(const Matrix& a, const Matrix& b);
    SLAU_STATUS solve_cramer();
    SLAU_STATUS solve_gauss();
    void print_solution() const;
    static void run_slau();
};