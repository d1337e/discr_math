#include <iostream>
#include "Matrix.h"

using namespace std;

int main(void) {
    int rows, cols = 0;
    cout << "Enter count of rows and cols in matrix(ex. 1 1, 3 5 e.t.c): ";
    cin >> rows >> cols;

    Matrix m(rows, cols);
    m.run_matrix();
    return 0;
}