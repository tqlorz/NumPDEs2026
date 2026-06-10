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
#include "FactorizationLU.hpp"

using namespace std;

class NewtonMethod {
private:
    vector<vector<double>> getJacobian(const function<Array(const Array&)>& F, 
                                        const Array& x, const double delta = 1e-10);
public:
    NewtonMethod() = default;
    ~NewtonMethod() = default;
    Array solve(const function<Array(const Array&)>& F, 
                const Array& x0, double tol = 1e-15, int maxIter = 100);
};

#endif