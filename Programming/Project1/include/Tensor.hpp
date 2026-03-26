/**
 * @file          Tensor.hpp
 * @author        zsh945
 * @date          2026-03-25
 * @brief         This file defines the Tensor class and related functions, like Kronecker product and matrix vectorization.
 */

#ifndef _TENSOR_
#define _TENSOR_

#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

/// @brief template class for tensor
/// @details This class is a data structure to store the tensor
/// @tparam T 
/// @tparam order 
template <typename T, int order>
class Tensor {
private:
    vector<T> _data;
    int _row, _col;
    int _size;
public:
    Tensor() : _row(0), _col(0), _size(0) {}
    Tensor(const int n) requires (order == 1);
    Tensor(const int row, const int col) requires (order == 2);
    void init(const int n) requires (order == 1);
    void init(const int row, const int col) requires (order == 2);
    T *data() {return &_data[0];}
    int size() const {return _size;}
    int row() const {return _row;}
    int col() const {return _col;}
    void resize(const int n);
    T& operator()(const int i) requires (order == 1) {return _data[i];}
    const T& operator()(const int i) const requires (order == 1) {return _data[i];}
    T& operator()(const int i, const int j) requires (order == 2) {return _data[i * _col + j];}
    const T& operator()(const int i, const int j) const requires (order == 2) {return _data[i * _col + j];}
};
    
/// @brief 1D Tensor of double
typedef Tensor<double, 1> Vector_Double;
/// @brief 1D Tensor of int
typedef Tensor<int, 1> Vector_Int;
/// @brief 2D Tensor of double
typedef Tensor<double, 2> Matrix_Double;
/// @brief 2D Tensor of int
typedef Tensor<int, 2> Matrix_Int;

template<typename T, int order>
Tensor<T, order> KroneckerProduct(const Tensor<T, order>& A, const Tensor<T, order>& B);

template<typename T, int order>
Tensor<T, 1> MatrixVectorization(const Tensor<T, order>& A);

template<typename T>
void PrintTensor(const Tensor<T, 1>& A);

template<typename T>
void PrintTensor(const Tensor<T, 2>& A);

// ===== detailed implementation of template class =====
/// @brief Constructor of vector
/// @tparam T 
/// @tparam order 
/// @param n 
template<typename T, int order>
Tensor<T, order>::Tensor(const int n) requires (order == 1) {
    init(n);
}

/// @brief Constructor of matrix
/// @tparam T 
/// @tparam order 
/// @param row 
/// @param col
template<typename T, int order>
Tensor<T, order>::Tensor(const int row, const int col) requires (order == 2) {
    init(row, col);
}

/// @brief Initialize the vector with given dimensions
/// @tparam T 
/// @tparam order 
/// @param n 
template<typename T, int order>
void Tensor<T, order>::init(const int n) requires (order == 1) {
    _row = n;
    _col = 1;
    _size = _row * _col;
    _data.resize(_size);
}

/// @brief Initialize the matrix with given dimensions
/// @tparam T 
/// @tparam order 
/// @param row 
/// @param col 
template<typename T, int order>
void Tensor<T, order>::init(const int row, const int col) requires (order == 2) {
    _row = row;
    _col = col;
    _size = _row * _col;
    _data.resize(_size);
}

/// @brief Resize the tensor
/// @tparam T
/// @tparam order
/// @param n
template<typename T, int order>
void Tensor<T, order>::resize(const int n) {
    _size = n;
    int dataSize = pow(n, order);
    _data.resize(dataSize);
}

// ===== detailed implementation of template function =====
/// @brief Compute the Kronecker product of two tensors
/// @tparam T 
/// @tparam order 
/// @param A 
/// @param B 
/// @return 
template<typename T, int order>
Tensor<T, order> KroneckerProduct(const Tensor<T, order>& A, const Tensor<T, order>& B) {
    int m = A.row();         // number of rows in A
    int n = A.col();         // number of columns in A
    int p = B.row();         // number of rows in B
    int q = B.col();         // number of columns in B

    Tensor<T, order> C(m * p, n * q);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < p; k++) {
                for (int l = 0; l < q; l++) {
                    C(i * p + k, j * q + l) = A(i, j) * B(k, l);
                }
            }
        }
    }
    return C;
}

/// @brief Create a column vector of size mn made of the columns of A stacked on top of one another from left to right
/// @tparam T 
/// @tparam order 
/// @param A 
/// @return 
template<typename T, int order>
Tensor<T, 1> MatrixVectorization(const Tensor<T, order>& A) {
    int m = A.row();         // number of rows in A
    int n = A.col();         // number of columns in A

    Tensor<T, 1> Vec(m * n);
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            Vec(j * m + i) = A(i, j);
        }
    }
    return Vec;
}

/// @brief Print the vector
/// @tparam T 
/// @param A 
template<typename T>
void PrintTensor(const Tensor<T, 1>& A) {
    for (int i = 0; i < A.size(); i++) {
        cout << A(i) << endl;
    }
}

/// @brief Print the matrix
/// @tparam T
/// @param A
template<typename T>
void PrintTensor(const Tensor<T, 2>& A) {
    for (int i = 0; i < A.row(); i++) {
        for (int j = 0; j < A.col(); j++) {
            cout << A(i, j) << " ";
        }
        cout << endl;
    }
}

#endif