#include "Matrix.h"

Matrix::Matrix(const int n, const int m) {
    if (n < 0 || m < 0) {
        throw std::invalid_argument("Can't create the matrix!");
    }
    rows = n;
    cols = m;
    matrix = std::vector<std::vector<double>>(n, std::vector<double>(m, 0.));
}

Matrix::Matrix(const int n, const int m, const double value) {
    if (n < 0 || m < 0) {
        throw std::invalid_argument("Can't create the matrix!");
    }
    rows = n;
    cols = m;
    matrix = std::vector<std::vector<double>>(n, std::vector<double>(m, value));
}

void Matrix::print_matrix() const {
    //std::cout << "===== Current Matrix =====\n";
    
    for (auto i = 0; i < rows; i++) {
        for (auto j = 0; j < cols; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}

Matrix operator+(const Matrix& a, const Matrix& b) {
    if (a.rows != b.rows || a.cols != b.cols) {
        throw std::invalid_argument("Can't add up the matrices");
    }
    
    Matrix new_matrix = Matrix(a.rows, a.cols);

    for (auto i = 0; i < new_matrix.rows; i++) {
        for (auto j = 0; j < new_matrix.cols; j++) {
            new_matrix.matrix[i][j] = a.matrix[i][j] + b.matrix[i][j];
        }
    }
    return new_matrix;
}

Matrix operator-(const Matrix& a, const Matrix& b) {
    if (a.rows != b.rows || a.cols != b.cols) {
        throw std::invalid_argument("Matrices can't be subtrackted");
    }

    Matrix new_matrix = Matrix(a.rows, a.cols);

    for (auto i = 0; i < new_matrix.rows; i++) {
        for (auto j = 0; j < new_matrix.cols; j++) {
            new_matrix.matrix[i][j] = a.matrix[i][j] - b.matrix[i][j];
        }
    }
    return new_matrix;
}

Matrix operator*(const Matrix& a, const Matrix& b) {
    if (a.cols != b.rows) {
        throw std::invalid_argument("Matrices can't be multiplied");
    }

    Matrix new_matrix = Matrix(a.rows, b.cols);

    for (auto i = 0; i < new_matrix.rows; i++) {
        for (auto j = 0; j < new_matrix.cols; j++) {
            double sum = 0.;
            for (auto k = 0; k < a.cols; k++) {
                sum += a.matrix[i][k] * b.matrix[k][j];
            }
            new_matrix.matrix[i][j] = sum;
        }
    }
    return new_matrix;
}

/* вот такие грабли */
Matrix operator*(const Matrix& a, const double value) {
    Matrix res = a;

    for (auto i = 0; i < res.rows; i++) {
        for (auto j = 0; j < res.cols; j++) {
            res.matrix[i][j] *= value;
        }
    }
    return res;
}

void Matrix::transposition() {
    std::vector<std::vector<double>> new_matrix(cols, std::vector<double>(rows));
    for (auto i = 0; i < this->rows; i++) {
        for (auto j = 0; j < this->cols; j++) {
            new_matrix[j][i] = matrix[i][j];
        }
    }
    matrix = new_matrix;
    std::swap(rows, cols);
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        this->cols = other.cols;
        this->rows = other.rows;
        this->matrix = other.matrix;
    }
    return *this;
}

double Matrix::det() const {
    if (rows != cols) {
        throw std::invalid_argument("Can't calculate determinant!");
    }
    
    if (rows == 0) return 1.0;
    if (rows == 1) return matrix[0][0];
    if (rows == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    std::vector<std::vector<double>> a = matrix;
    int n = rows;
    int swap_count = 0;

    for (int col = 0; col < n; ++col) {
        
        int pivot_row = col;
        double max_val = std::abs(a[col][col]);
        
        for (int row = col + 1; row < n; ++row) {
            double val = std::abs(a[row][col]);
            if (val > max_val) {
                max_val = val;
                pivot_row = row;
            }
        }
        
        if (max_val < std::numeric_limits<double>::epsilon()) {
            return 0.;
        }
        
        if (pivot_row != col) {
            std::swap(a[col], a[pivot_row]);
            swap_count++;
        }
        
        for (int row = col + 1; row < n; ++row) {
            double factor = a[row][col] / a[col][col];
            
            for (int j = col; j < n; ++j) {
                a[row][j] -= factor * a[col][j];
            }
        }
    }

    double determinant = 1.;
    for (int i = 0; i < n; ++i) {
        determinant *= a[i][i];
    }

    if (swap_count % 2 == 1) {
        determinant = -determinant;
    }

    return determinant;
}

int Matrix::rank() const {
    if (cols == 0 || rows == 0) return 0;
    int r = 0;
    std::vector<std::vector<double>> a = matrix;

    for (auto col = 0; col < cols && r < rows; col++) {
        int pivot = -1;
        for (auto i = r; i < rows; i++) {
            if (std::abs(a[i][col]) > std::numeric_limits<double>::epsilon()) {
                pivot = i;
                break;
            }
        }
        if (pivot == -1) continue;

        if (pivot != r) {
            std::swap(a[r], a[pivot]);
        }

        for (auto i = r + 1; i < rows; i++) {
            double factor = a[i][col] / a[r][col];
            for (auto j = col; j < cols; j++) {
                a[i][j] -= factor * a[r][j];
            }
        }
        r++;
    }
    return r;
}

void Matrix::fill_from_user() {
    std::cout << "Filling matrix manually [" << rows << "; " << cols << "]\n";
    for (auto i = 0; i < rows; i++) {
        for (auto j = 0; j < cols; j++) {
            double value = 0.;
            std::cout << "a[" << i << " " << j << "] = ";
            while (!(std::cin >> value)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input\n";
                std::cout << "a[" << i << " " << j << "] = ";
            }
            matrix[i][j] = value;
        }
    }
    std::cout << "Success filling\n\n";
}

void Matrix::fill_random(const double min, const double max) {
    std::cout << "Filling matrix random [" << rows << "; " << cols << "]\n";

    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::uniform_real_distribution<double> dist(min, max);
    for (auto i = 0; i < rows; i++) {
        for (auto j = 0; j < cols; j++) {
            matrix[i][j] = dist(gen);
        }
    }
    std::cout << "Success filling\n\n";
}

Matrix Matrix::minor(const int row_del, const int col_del) const {
    if (rows != cols) {
        throw std::invalid_argument("Minor only for NxN matrices");
    }

    Matrix m(rows - 1, cols - 1);
    int r = 0;
    for (auto i = 0; i < rows; i++) {
        if (i == row_del) continue;
        int c = 0;
        for (auto j = 0; j < cols; j++) {
            if (j == col_del) continue;
            m.matrix[r][c] = this->matrix[i][j];
            c++;
        }
        r++;
    }
    return m;
}

// убрал const, так как требовали,
// чтобы обратная матрица сразу нашу меняла, а я думал что нашу лучше не трогать
void Matrix::inverse_by_form() {
    std::cout << "================ Inverse Kramer ================\n";
    const double DET = this->det();
    if (cols != rows || std::abs(DET) < std::numeric_limits<double>::epsilon()) {
        throw std::invalid_argument("Can't find inverse matrix");
    }

    Matrix algebra_dop(rows, cols);
    for (auto i = 0; i < rows; i++) {
        for (auto j = 0; j < cols; j++) {
            double sign = ((i + j) % 2 == 0) ? 1. : -1.;
            algebra_dop.matrix[i][j] = sign * minor(i, j).det();
        }
    }

    algebra_dop.transposition();

    for (auto i = 0; i < rows; i++) {
        for (auto j = 0; j < cols; j++) {
            algebra_dop.matrix[i][j] /= DET;
        }
    }

    algebra_dop.print_matrix();
    *this = algebra_dop;
}

// убрал const, так как требовали,
// чтобы обратная матрица сразу нашу меняла, а я думал что нашу лучше не трогать
void Matrix::inverse_by_regular() {
    std::cout << "================ Inverse Regular ================\n";
    const double DET = this->det();
    if (cols != rows || std::abs(DET) < std::numeric_limits<double>::epsilon()) {
        throw std::invalid_argument("Can't find inverse matrix");
    }
    const int n = rows;

    std::vector<std::vector<double>> ai(n, std::vector<double>(2 * n, 0.));

    for (auto i = 0; i < n; i++) {
        for (auto j = 0; j < n; j++) {
            ai[i][j] = matrix[i][j];
        }
        ai[i][n + i] = 1.;
    }

    for (auto i = 0; i < n; i++) {
        int pivot = i;
        for (auto k = i + 1; k < n; k++) {
            if (std::abs(ai[k][i]) > std::abs(ai[pivot][i])) {
                pivot = k;
            }
        }
        if (pivot != i) {
            std::swap(ai[i], ai[pivot]);
        }

        

        const double DIV = ai[i][i];
        for (auto j = 0; j < 2 * n; j++) {
            ai[i][j] /= DIV;
        }
        
        for (auto k = 0; k < n; k++) {
            if (k == i) continue;

            const double FACTOR = ai[k][i];
            for (auto j = 0; j < 2 * n; j++) {
                ai[k][j] -= FACTOR * ai[i][j];
            }
        }

    }

    Matrix inv(n, n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            inv.matrix[i][j] = ai[i][j + n];
    
    inv.print_matrix();
    *this = inv;
}

void Matrix::run_matrix() {
    std::cout << "===== WELCOME TO MATRIX LIB =====\n";
    std::cout << "Current matrix size [" << rows << "x" << cols << "]\n";
    std::cout << "Mode of filling matrix\n" << MANUAL << ". Manual filling\n"
            << RANDOM << ". Random filling\n: ";
    int choice = 0;
    std::cin >> choice;
        
    switch (choice) {
        case MANUAL: {
            this->fill_from_user();
            break;
        }
        case RANDOM: {
            this->fill_random();
            break;
        }
        default:
            throw std::invalid_argument("Invalid option of filling!");
    }

    this->print_matrix();
    while (true) {
        std::cout << "\nChoose option with matrix\n"
                << ADDITION << ". Addition (+)\n"
                << SUBSTRACTION << ". Substraction (-)\n"
                << MULTIPLICATION << ". Multiplication (*)\n"
                << MULTIPLICATION_SCALAR << ". Multiplication with scalar (*)\n"
                << DETERMINATE << ". Find determinate of matrix\n"
                << RANK << ". Find rank of matrix\n"
                << INVERSEFORM << ". Find inverse matrix by formula\n"
                << INVERSEGAY << ". Find inverse matrix by regular expressions\n"
                << EXIT << ". Exit\n";
        choice = 0;
        std::cin >> choice;

        switch (choice) {
            case ADDITION: {
                std::cout << "You chose (+)\n";
                std::cout << "Which matrix (+) [curr size = " << rows << "x" << cols << "]\n";
                Matrix temp(rows, cols);
                temp.fill_from_user();
                std::cout << "---Second matrix\n";
                temp.print_matrix();
                *this = *this + temp;
                std::cout << "Result: \n";
                this->print_matrix();
                break;
            }
            case SUBSTRACTION: {
                std::cout << "You chose (-)\n";
                std::cout << "Which matrix (-) [curr size = " << rows << "x" << cols << "]\n";
                Matrix temp(rows, cols);
                temp.fill_from_user();
                std::cout << "---Second matrix\n";
                temp.print_matrix();
                *this = *this - temp;
                std::cout << "Result: \n";
                this->print_matrix();
                break;
            }
            case MULTIPLICATION: {
                std::cout << "You chose (*)\n";
                int b_rows, b_cols = 0;
                std::cout << "Enter razmeri of matrix (dolzhno bit' " << cols << " rows): ";
                std::cin >> b_rows >> b_cols;

                if (b_rows != this->cols) {
                    std::cout << "Error! Can't multiplicate matrix!\n";
                    std::cout << "Current matrix: " << rows << "x" << cols << '\n';
                    std::cout << "Second matrix must have " << cols << " rows!!!\n";
                    break;
                }
                
                Matrix temp(b_rows, b_cols);
                temp.fill_from_user();
                std::cout << "---Second matrix\n";
                temp.print_matrix();

                *this = *this * temp;
                std::cout << "Result: \n";
                this->print_matrix();
                break;
            }
            case MULTIPLICATION_SCALAR: {
                std::cout << "You chose multiplication on scalar (*)\n";
                std::cout << "Enter value: ";
                double value = 0.;
                std::cin >> value;
                *this = *this * value;
                this->print_matrix();
                break;
            }
            case DETERMINATE: {
                std::cout << "You chose determinate\n";
                std::cout << "Determinate = " << this->det() << "\n";
                break;
            }
            case RANK: {
                std::cout << "You chose rank\n";
                std::cout << "Rank = " << this->rank() << "\n";
                break;
            }
            case INVERSEFORM: {
                std::cout << "You chose inverse by formyla\n";
                std::cout << "Inverse:\n";
                this->inverse_by_form();
                break;
            }
            case INVERSEGAY: {
                std::cout << "You chose inverse by regular\n";
                std::cout << "Inverse:\n";
                this->inverse_by_regular();
                break;
            }
            case EXIT: {
                std::cout << "Exiting..\n";
                return;
            }
            default:
                std::cout << "Unknow operation!\n";
                continue;
        }
    }
}

int Matrix::get_cols() const {
    return this->cols;
}

int Matrix::get_rows() const {
    return this->rows;
}

double& Matrix::operator()(const int i, const int j) {
    return matrix[i][j];
}

double Matrix::operator()(const int i, const int j) const {
    return matrix[i][j];
}