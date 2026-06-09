/**
 * @file NewtonMethod.cpp
 * @author zsh945
 * @date 2026-06-08
 */

#include "NewtonMethod.hpp"

// Array NewtonMethod::solve(const function<Array(const Array&)>& F, 
//                             const function<Array(const Array&)>& J,
//                             const Array& x0, double tol = 1e-6, int maxIter = 100) {
    // LinfNorm linfNorm;
    // Array x = x0;
    // for (int iter = 0; iter < maxIter; ++iter) {
    //     Array Fx = F(x);
    //     if (linfNorm(Fx) < tol) {
    //         cout << "Converged in " << iter << " iterations." << endl;
    //         return x;
    //     }
    //     Array Jx = J(x);
    //     // Solve Jx * dx = -Fx for dx
    //     // Here we can use a simple linear solver, e.g., Gaussian elimination
    //     // For simplicity, we will just use a placeholder function solveLinearSystem
    //     Array dx = solveLinearSystem(Jx, -Fx);
    //     x += dx;
    // }
    // cout << "Max iterations reached without convergence." << endl;
    // return x;
// }