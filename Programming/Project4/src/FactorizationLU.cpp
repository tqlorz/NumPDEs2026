/**
 * @file FactorizationLU.cpp
 * @author zsh945 
 * @date 2026-06-08
 */

#include "FactorizationLU.hpp"
#include <cmath>
#include <stdexcept>

void FactorizationLU::factorize(const vector<vector<double>>& A) {
    const int n = static_cast<int>(A.size());
    for (const auto& row : A) {
        if (static_cast<int>(row.size()) != n) {
            throw invalid_argument("Error in FactorizationLU::factorize: matrix must be square.");
        }
    }

    _L.assign(n, vector<double>(n, 0.0));
    _U = A;
    _pivots.resize(n);
    for (int i = 0; i < n; ++i) {
        _pivots[i] = i;
        _L[i][i] = 1.0;
    }

    for (int k = 0; k < n; ++k) {
        int pivot = k;
        double pivotAbs = std::abs(_U[k][k]);
        for (int i = k + 1; i < n; ++i) {
            const double candidate = std::abs(_U[i][k]);
            if (candidate > pivotAbs) {
                pivotAbs = candidate;
                pivot = i;
            }
        }
        if (pivotAbs < 1e-14) {
            throw runtime_error("Error in FactorizationLU::factorize: singular matrix.");
        }

        if (pivot != k) {
            swap(_U[k], _U[pivot]);
            swap(_pivots[k], _pivots[pivot]);
            for (int j = 0; j < k; ++j) {
                swap(_L[k][j], _L[pivot][j]);
            }
        }

        for (int i = k + 1; i < n; ++i) {
            const double factor = _U[i][k] / _U[k][k];
            _L[i][k] = factor;
            _U[i][k] = 0.0;
            for (int j = k + 1; j < n; ++j) {
                _U[i][j] -= factor * _U[k][j];
            }
        }
    }
}

vector<double> FactorizationLU::solve(const vector<double>& b) const {
    const int n = static_cast<int>(_L.size());
    if (static_cast<int>(b.size()) != n) {
        throw invalid_argument("Error in FactorizationLU::solve: right-hand side size mismatch.");
    }

    vector<double> rhs(n), y(n), x(n);
    for (int i = 0; i < n; ++i) {
        rhs[i] = b[_pivots[i]];
    }

    // Forward substitution to solve Ly = b
    for (int i = 0; i < n; ++i) {
        y[i] = rhs[i];
        for (int j = 0; j < i; ++j) {
            y[i] -= _L[i][j] * y[j];
        }
        y[i] /= _L[i][i];
    }

    // Backward substitution to solve Ux = y
    for (int i = n - 1; i >= 0; --i) {
        x[i] = y[i];
        for (int j = i + 1; j < n; ++j) {
            x[i] -= _U[i][j] * x[j];
        }
        if (std::abs(_U[i][i]) < 1e-14) {
            throw runtime_error("Error in FactorizationLU::solve: singular upper-triangular system.");
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
