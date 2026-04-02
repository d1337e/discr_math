#include "slau.h"

SLAU::SLAU(const Matrix& a, const Matrix& b) : a(a), b(b), x(a.get_cols(), 1), solve(NOT_SOLVED) {
    // кол-во уравнений
    const int m = a.get_rows();
    // кол-во неизвестных
    const int n = a.get_cols();
    
    if (m != b.get_rows()) {
        throw std::invalid_argument("Col-vo uravnenii ne sovpadaet s B");
    }

    if (b.get_cols() != 1) {
        throw std::invalid_argument("B eto vector, u nego 1 stolbec dolzhen bit'!");
    }

    std::cout << "Success initialized constructor!\n";
}

SLAU_STATUS SLAU::solve_cramer() {
    const int m = a.get_rows();
    const int n = a.get_cols();

    if (m != n) {
        std::cout << ("[CRAMER](rows != cols) => can't solve!");
        solve = CRAMER_NEMA;
        return solve;
    }

    const double det_a = a.det();
    if (std::abs(det_a) < std::numeric_limits<double>::epsilon()) {
        std::cout << ("[CRAMER] determinant is zero!");
        solve = CRAMER_NEMA;
        return solve;
    }

    std::cout << "[CRAMER] det =" << det_a << ". Solving...\n";

    for (auto i = 0; i < n; i++) {
        Matrix ai = a;
        // заменяем столбец i на вектор b
        for (auto row = 0; row < n; row++) {
            ai(row, i) = b(row, 0);
        }

        const double det_ai = ai.det();

        x(i, 0) = det_ai / det_a;
    }

    solve = UNIQUE_SOLUTION;
    std::cout << "[CRAMER] Success!\n";
    return solve;
}

void SLAU::print_solution() const {
    if (solve == NOT_SOLVED) {
        std::cout << "System not solved yet.\n";
        return;
    }
    
    if (solve == NO_SOLUTION) {
        std::cout << "\n=== Result ===\n";
        std::cout << "System has NO SOLUTIONS (inconsistent).\n";
        std::cout << "===============\n";
        return;
    }
    
    if (solve == INFINITE_SOLUTIONS) {
        std::cout << "\n=== Result ===\n";
        std::cout << "System has INFINITE SOLUTIONS.\n";
        std::cout << "Free variables exist.\n";
        std::cout << "===============\n";
        return;
    }

    if (solve == CRAMER_NEMA) {
        std::cout << "Cramer method not applicable.\n";
        return;
    }
    
    // UNIQUE_SOLUTION
    std::cout << "\n=== Solution ===\n";
    for (int i = 0; i < x.get_rows(); ++i) {
        std::cout << "x[" << i << "] = " << x(i, 0) << "\n";
    }
    std::cout << "===============\n";
}

SLAU_STATUS SLAU::solve_gauss() {
    const int m = a.get_rows();
    const int n = a.get_cols();

    std::cout << "[GAUSS] Matrix size: " << m << "x" << n << '\n';
    Matrix ab(m, n + 1);

    for (auto i = 0; i < m; i++) {
        for (auto j = 0; j < n; j++) {
            ab(i, j) = a(i, j);
        }
    }

    for (auto i = 0; i < b.get_rows(); i++) {
        ab(i, n) = b(i, 0);
    }

    
    int row = 0;
    for (auto col = 0; col < n && row < m; col++) {
        // находим главный элемент
        int pivot = row;
        for (auto i = row + 1; i < m; i++) {
            if (std::abs(ab(i, col)) > std::abs(ab(pivot, col))) {
                pivot = i;
            }
        }

        if (std::abs(ab(pivot, col)) < std::numeric_limits<double>::epsilon()) {
            continue;
        }

        if (pivot != row) {
            for (auto j = 0; j <= n; j++) {
                std::swap(ab(row, j), ab(pivot, j));
            }
        }

        for (auto i = row + 1; i < m; i++) {
            const double FACT = ab(i, col) / ab(row, col);
            for (auto j = col; j <= n; j++) {
                ab(i, j) -= FACT * ab(row, j);
            }
        }
        row++;
    }

    const int rank = row;

    for (auto i = rank; i < m; i++) {
        if (std::abs(ab(i, n)) > std::numeric_limits<double>::epsilon()) {
            std::cout << "[GAUSS] No soulutions!\n";
            solve = NO_SOLUTION;
            return solve;
        }
    }

    if (rank < n) {
        std::cout << "[GAUSS] Infinite solutions (Rank " << rank << " < Vars " << n << ")\n";
        solve = INFINITE_SOLUTIONS;
        return solve; 
    }

    std::cout << "[GAUSS] Unique solution found\n";

    for (auto i = rank - 1; i >= 0; i--) {
         double sum = ab(i, n);
        for (auto j = i + 1; j < n; j++) {
            sum -= ab(i, j) * x(j, 0);
        }

        x(i, 0) = sum / ab(i, i);
    }

    solve = UNIQUE_SOLUTION;
    std::cout << "[GAUSS] Success!\n";
    return solve;

}

void SLAU::run_slau() {
    std::cout << "======================================\n";
    std::cout << "             SLAU SOLVER              \n";
    std::cout << "======================================\n";

    std::cout << "\nChoose mode\n"
              << CRAMER << ". Cramer\n"
              << GAUSS << ". Gauss\n";
    size_t choice = 0;
    std::cin >> choice;
    switch (choice)
    {
    case CRAMER: {
        std::cout << "CRAMER MODE\n";
        int rows, cols = 0;
        std::cout << "Enter count of rows and cols in matrix(ex. 1 1, 3 5 e.t.c): ";
        std::cin >> rows >> cols;

        Matrix a(rows, cols);
        std::cout << "---> Fill matrix A\n";
        a.fill_from_user();
        a.print_matrix();

        std::cout << "\n---> Fill vector B\n";
        Matrix b(rows, 1);
        b.fill_from_user();
        b.print_matrix();

        SLAU system(a, b);
        SLAU_STATUS status = system.solve_cramer();
        system.print_solution();
        std::cout << "\n";
        break;
    }
    
    case GAUSS: {
        std::cout << "GAUSS MODE\n";
        int rows, cols = 0;
        std::cout << "Enter count of rows and cols in matrix(ex. 1 1, 3 5 e.t.c): ";
        std::cin >> rows >> cols;

        Matrix a(rows, cols);
        std::cout << "---> Fill matrix A\n";
        a.fill_from_user();
        a.print_matrix();

        std::cout << "\n---> Fill vector B\n";
        Matrix b(rows, 1);
        b.fill_from_user();
        b.print_matrix();

        SLAU system(a, b);
        SLAU_STATUS status = system.solve_gauss();
        system.print_solution();
        std::cout << "\n";
        break;
    }

    default:
        throw std::invalid_argument("Error choice in mode!");
    }
}