/**
 * @file NewtonMethod.cpp
 * @author zsh945
 * @date 2026-06-08
 */

#include "NewtonMethod.hpp"

Array NewtonMethod::solve(const function<Array(const Array&)>& F, 
                            const Array& x0, double tol, int maxIter) {
    LinfNorm linfNorm;
    Array x = x0;
    for (int iter = 0; iter < maxIter; ++iter) {
        Array Fx = F(x);
        if (linfNorm(Fx) < tol) {
            // cout << "Converged in " << iter << " iterations." << endl;
            return x;
        }
        vector<vector<double>> Jx = getJacobian(F, x);
        // Solve Jx * dx = -Fx for dx
        FactorizationLU lu;
        lu.factorize(Jx);
        Array rhs = -1.0 * Fx;
        vector<double> rhs_vec(rhs.begin(), rhs.end());
        vector<double> dx_vec = lu.solve(rhs_vec);
        Array dx(dx_vec.begin(), dx_vec.end());
        x += dx;
    }
    cout << "Max iterations reached without convergence" << endl;
    return x;
}

vector<vector<double>> NewtonMethod::getJacobian(const function<Array(const Array&)>& F, 
                                                    const Array& x, const double delta) {
    int n = x.size();
    vector<vector<double>> J(n, vector<double>(n, 0.0));
    for (int j = 0; j < n; ++j) {
        double h = delta * max(1.0, abs(x[j]));
        Array x_plus = x;
        Array x_minus = x;
        x_plus[j] += h;
        x_minus[j] -= h;
        Array F_plus = F(x_plus);
        Array F_minus = F(x_minus);
        for (int i = 0; i < n; ++i) {
            J[i][j] = (F_plus[i] - F_minus[i]) / (2 * h);
        }
    }
    return J;
}
