/**
 * @file FactorizationLU.cpp
 * @author zsh945 
 * @date 2026-06-08
 */

#include "FactorizationLU.hpp"

void FactorizationLU::factorize(const vector<vector<double>>& A) {
    int n = A.size();
    _L.resize(n, vector<double>(n, 0.0));
    _U.resize(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i <= j) {
                _U[i][j] = A[i][j];
                for (int k = 0; k < i; ++k) {
                    _U[i][j] -= _L[i][k] * _U[k][j];
                }
            } else {
                _L[i][j] = A[i][j];
                for (int k = 0; k < j; ++k) {
                    _L[i][j] -= _L[i][k] * _U[k][j];
                }
                _L[i][j] /= _U[j][j];
            }
        }
    }
}

vector<double> FactorizationLU::solve(const vector<double>& b) const {
    int n = _L.size();
    vector<double> y(n), x(n);

    // Forward substitution to solve Ly = b
    for (int i = 0; i < n; ++i) {
        y[i] = b[i];
        for (int j = 0; j < i; ++j) {
            y[i] -= _L[i][j] * y[j];
        }
    }

    // Backward substitution to solve Ux = y
    for (int i = n - 1; i >= 0; --i) {
        x[i] = y[i];
        for (int j = i + 1; j < n; ++j) {
            x[i] -= _U[i][j] * x[j];
        }
        x[i] /= _U[i][i];
    }

    return x;
}

vector<vector<double>> FactorizationLU::inv() const {
    int n = _L.size();
    vector<vector<double>> A_inv(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; ++i) {
        vector<double> e(n, 0.0);
        e[i] = 1.0;
        vector<double> x = solve(e);
        for (int j = 0; j < n; ++j) {
            A_inv[j][i] = x[j];
        }
    }
    return A_inv;
}