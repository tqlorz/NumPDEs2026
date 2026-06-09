/**
 * @file NewtonMethod.hpp
 * @author zsh945 
 * @date 2026-06-08
 */

#ifndef _NEWTONMETHOD_
#define _NEWTONMETHOD_

#include <iostream>
#include "Function.hpp"
#include "Array.hpp"
#include "Norm.hpp"

using namespace std;

class NewtonMethod {
private:
    // Array solveLinearSystem(const vector<vector<double>>& A, const Array& b);
public:
    NewtonMethod() = default;
    ~NewtonMethod() = default;
    // Array solve(const function<Array(const Array&)>& F, 
    //             const function<Array(const Array&)>& J,
    //             const Array& x0, double tol = 1e-6, int maxIter = 100);
};

#endif