/**
 * @file FactorizationLU.hpp
 * @author zsh945 
 * @date 2026-06-08
 */

#ifndef _FACTORIZATIONLU_
#define _FACTORIZATIONLU_

#include <vector>
#include <iostream>

using namespace std;

/// @brief LU factorization class for solving linear systems and computing inverses
class FactorizationLU {
private:
    vector<vector<double>> _L;
    vector<vector<double>> _U;
    vector<int> _pivots;
public:
    FactorizationLU() = default;
    ~FactorizationLU() = default;
    void factorize(const vector<vector<double>>& A);
    vector<double> solve(const vector<double>& b) const;
    vector<vector<double>> inv() const;
    const vector<vector<double>>& L() const { return _L; }
    const vector<vector<double>>& U() const { return _U; }
};

#endif
