/* 
 * EECS 348 – Software Engineer I
 * Lab 11: Even More C++ Programming
 * File: main.cpp
 * Author: Cole Adams
 * 
 * Description:
 *  This program reads in two NxN matrices from a file and performs basic operations:
 *  1. Adding matrices (A + B).
 *  2. Multiplying matrices (AB and BA).
 *  3. Summing main and secondary diagonals.
 *  4. Swapping rows and columns.
 *  5. Changing an element at a given index.
 *  This program is realized with a Matrix class, utilizing operator overloading.
 *  The program will validate input and print all results.
 */

// Assumptions per lab instructions:
// - N is a valid positive integer.
// - Matrices are NxN; two matrices follow N in the input file.
// Validation used:
// - File I/O is checked (open/read success; enough integers for Matrices).
// - All dimensions checks done in main(), outside of the class Matrix.
//  -> Validation is separated from the raw data storage and calculations (I think like real types such as int[]).

#include <iostream> // Console I/O; std::cout, std::cin.
#include <fstream>  // Reading from files; std::ifstream.
#include <string>   // Working with strings; std::string.
#include <iomanip>  // Formatting output; std::setw().

// MxN Matrix of integers.
// Includes some operator overloading and basic computations.
class Matrix {
public:
    // Basic constructor; creates an MxN Matrix with elements initialized as 0.
    // Assumes inputs rows and cols >= 0.
    Matrix(int rows, int cols) {
        // Set rows_ and cols_.
        rows_ = rows;
        cols_ = cols;

        if (rows > 0 && cols_ > 0) {
            // Set data_ to an array of size rows_ * cols_, initialized with 0s.
            data_ = new int[rows_ * cols_]();
        } else {
            // If rows_ or cols_ == 0, the Matrix cannot hold elements -> keep data_ == nullptr.
            data_ = nullptr;
        }
    }

    // Construct a Matrix from another Matrix (deep copy).
    Matrix(const Matrix& other) {
        // Set rows_ and cols_.
        rows_ = other.rows_;
        cols_ = other.cols_;

        if (rows_ > 0 && cols_ > 0) {
            // Set data_ to an array of size rows_ * cols_;
            data_ = new int[rows_ * cols_];

            // Copy each element from the other Matrix.
            for (int i = 0; i < rows_ * cols_; ++i)
                data_[i] = other.data_[i];
        } else {
            // If rows_ or cols_ == 0, the Matrix cannot hold elements -> keep data_ == nullptr.
            data_ = nullptr;
        }
    }

    // Copy-swap. Allows for reassigning of an already initialized Matrix.
    Matrix& operator=(Matrix other) {
        swap(other);
        return *this;
    }

    // Exchange all data members of Matrices *this and other.
    void swap (Matrix& other) {
        std::swap(rows_, other.rows_);
        std::swap(cols_, other.cols_);
        std::swap(data_, other.data_);
    }

    // Non-member swap (labeled friend for direct data access).
    // Allows the standard library and user code to call swap(A, B)
    // in a generic way, which internally calls the member swap().
    friend void swap(Matrix& A, Matrix& B) {
        A.swap(B);
    }

    // Deconstructor to safely release dynamically allocated memory.
    // Called automatically when a Matrix goes out of scope.
    ~Matrix() {
        delete[] data_;
    }

    // Element access operators.
    // Allows use of parentheses like M(i,j) to get or modify elements.
    // The const version is for when the Matrix is read-only.
    int& operator()(int row, int col) {
        return data_[row * cols_ + col];
    }
    const int& operator()(int row, int col) const {
        return data_[row * cols_ + col];
    }

    // Adds two Matrices (assumes same dimensions). Returns resulting Matrix.
    // Uses operator overloading (e.g., A + B).
    Matrix operator+(const Matrix& other) const {
        Matrix result(rows_, cols_);
        for (int i = 0; i < rows_; ++i)
            for (int j = 0; j < cols_; ++j)
                result(i, j) = (*this)(i, j) + other(i, j);
        return result;
    }

    // Multiplies this Matrix by another Matrix using standard Matrix multiplication.
    // Returns resulting Matrix.
    // Uses operator overloading (e.g., A * B).
    Matrix operator*(const Matrix& other) const {
        Matrix result(rows_, other.cols_);
        for (int i = 0; i < rows_; ++i)
            for (int j = 0; j < other.cols_; ++j) {
                int sum = 0;
                for (int k = 0; k < cols_; ++k)
                    sum += (*this)(i, k) * other(k, j);
                result(i, j) = sum;
            }
        return result;
    }

    // Computes and returns the sum of the main diagonal (top-left to bottom-right).
    // Assumes Matrix is a square matrix.
    int mainDiagonalSum() const {
        int sum = 0;
        for (int i = 0; i < rows_; ++i) sum += (*this)(i, i);
        return sum;
    }

    // Computes and returns the sum of the secondary diagonal (top-right to bottom-left).
    // Assumes Matrix is a square matrix.
    int secondaryDiagonalSum() const {
        int sum = 0;
        for (int i = 0; i < rows_; ++i) sum += (*this)(i, rows_-1-i);
        return sum;
    }

    // Returns a new Matrix with rows r1 and r2 swapped (assumes r1 and r2 are valid indices).
    // The original Matrix is not modified.
    // Defaults to r1 = 0 and r2 = 1 if no indices are given.
    Matrix swapRows(int r1 = 0, int r2 = 1) const {
        Matrix result(*this);
        for (int j = 0; j < cols_; ++j)
            std::swap(result(r1, j), result(r2, j));
        return result;
    }

    // Returns a new Matrix with columns c1 and c2 swapped (assumes c1 and c2 are valid indices).
    // The original Matrix is not modified.
    // Defaults to c1 = 0 and c2 = 1 if no indices are given.
    Matrix swapCols(int c1 = 0, int c2 = 1) const {
        Matrix result(*this);
        for (int i = 0; i < rows_; ++i)
            std::swap(result(i, c1), result(i, c2));
        return result;
    }

    // Returns a new Matrix with the element at (row, col) set to 'value'.
    // Assumes value to be an integer and (row, col) to be a valid index.
    Matrix set(int row = 0, int col = 0, int value = 100) const {
        Matrix result(*this);
        result(row, col) = value;
        return result;
    }

    // Accessor functions that return the number of rows and columns in the Matrix.
    int rows() const { return rows_; }
    int cols() const { return cols_; }

private:
    int rows_ {0};          // Private variable for integer M rows; default is 0.
    int cols_ {0};          // Private variable for integer N cols; default is 0.
    int* data_ {nullptr};   // Private variable for integer pointer to matrix data; default is nullptr.
};

// Overloaded stream insertion operator for formatted output.
// Prints the Matrix with neatly aligned columns using std::setw(4).
// Allows for lines like: std:: cout << A;
// Allows for chaining like: std::cout << A << B;
std::ostream& operator<<(std::ostream& os, const Matrix& M) {
    for (int i = 0; i < M.rows(); ++i) {
        for (int j = 0; j < M.cols(); ++j)
            os << std::setw(4) << M(i, j);
        os << '\n';
    }
    return os;
}

// Writes values from an input stream to a Matrix.
// Returns true if the matrix is filled without issue.
// Returns false otherwise.
bool readSquareMatrix(std::istream& in, Matrix& M) {
    for (int i = 0; i < M.rows(); ++i)
        for (int j = 0; j < M.cols(); ++j)
            if (!(in >> M(i,j))) return false;
    return true;
}

// Program entry point.
int main() {
    // Receive the input filename from the user.
    std::string filename;
    std::cout << "Enter the input filename (include .txt): ";
    std::cin >> filename;

    // Attempt to open the file.
    std::ifstream file (filename);

    // Verify that the file is open.
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file '" << filename << "'.\n";
        return 1;
    }

    // Initialize size N.
    int N;

    // Verify valid size N for NxN matrices.
    if (!(file >> N)) {
        std::cerr << "Error: Could not read size N from the file.\n";
        return 1;
    }
    if (N <= 0) {
        std::cerr << "Error: Size N must be a positive integer.\n";
        return 1;
    }

    // Initialize matrices A and B.
    Matrix A(N, N);
    Matrix B(N, N);

    // Attempt to write matrices from the input file.
    if (!readSquareMatrix(file, A) || !readSquareMatrix(file, B)) {
        std::cerr << "Error: File did not contain 2 NxN matrices of integers.";
        return 1;
    }

    // Close the file when done reading.
    file.close();
    
    // Displaying results.

    // Original Matrices read.
    std::cout << "Matrix A:\n" << A << "\n";
    std::cout << "Matrix B:\n" << B << "\n";

    // Addition (A + B).
    std::cout << "A + B:\n";
    if (A.rows() == B.rows() && A.cols() == B.cols()) {
        std::cout << A + B << "\n";
    } else {
        std::cout << "Notice: Dimension mismatch; cannot add matrices.\n\n";
    }

    // Multiplication (AB and BA).
    std::cout << "AB:\n";
    if (A.cols() == B.rows()) {
        std::cout << A * B << "\n";
    } else {
        std::cout << "Notice: Dimension mismatch; cannot multiply AB.\n\n";
    }
    std::cout << "BA:\n";
    if (B.cols() == A.rows()) {
        std::cout << B * A << "\n";
    } else {
        std::cout << "Notice: Dimension mismatch; cannot multiply BA.\n\n";
    }

    // Diagonal sums (A).
    std::cout << "Diagonal sums of A:\n";
    if (A.rows() == A.cols()) {
        std::cout << "Main diagonal: " << A.mainDiagonalSum() << "\n";
        std::cout << "Secondary diagonal: " << A.secondaryDiagonalSum() << "\n\n";
    } else {
        std::cout << "Notice: Cannot sum diagonals of non-square matrix.";
    }

    // Swapping rows (A).
    std::cout << "Swapping rows of A:\n";

    // Receiving indices.
    int r1, r2;
    std::cout << "Enter r1: ";
    std::cin >> r1;
    std::cout << "Enter r2: ";
    std::cin >> r2;
    std::cout << "\n";

    // Swapping.
    if (r1 < 0 || r1 >= A.rows()) {
        std::cout << "Error: Invalid row index r1 = " << r1 << ".\n\n";
    } else if (r2 < 0 || r2 >= A.rows()) {
        std::cout << "Error: Invalid row index r2 = " << r2 << ".\n\n";
    } else {
        std::cout << "Original matrix:\n" << A << "\n";
        std::cout << "Modified matrix:\n" << A.swapRows(r1, r2) << "\n";
    }

    // Swapping columns (A).
    std::cout << "Swapping columns of A:\n";

    // Receiving indices.
    int c1, c2;
    std::cout << "Enter c1: ";
    std::cin >> c1;
    std::cout << "Enter c2: ";
    std::cin >> c2;
    std::cout << "\n";

    // Swapping.
    if (c1 < 0 || c1 >= A.cols()) {
        std::cout << "Error: Invalid column index c1 = " << c1 << ".\n\n";
    } else if (c2 < 0 || c2 >= A.cols()) {
        std::cout << "Error: Invalid column index c2 = " << c2 << ".\n\n";
    } else {
        std::cout << "Original matrix:\n" << A << "\n";
        std::cout << "Modified matrix:\n" << A.swapCols(c1, c2) << "\n";
    }

    // Updating element (A).
    std::cout << "Updating element of A:\n";

    // Receiving indices.
    int r, c, value;
    std::cout << "Enter row index: ";
    std::cin >> r;
    std::cout << "Enter column index: ";
    std::cin >> c;
    std::cout << "Enter element value: ";
    std::cin >> value;
    std::cout << "\n";

    // Updating.
    if (r < 0 || r >= A.rows() || c < 0 || c >= A.cols()) {
        std::cout << "Error: Invalid index (" << r << "," << c << ").\n\n";
    } else {
        std::cout << "Original matrix:\n" << A << "\n";
        std::cout << "Modified matrix:\n" << A.set(r, c, value) << "\n";
    }

    // Program ended successfully.
    std::cout << "Program complete. Ending...\n";
    return 0;
}