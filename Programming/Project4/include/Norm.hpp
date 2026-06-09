/**
 * @file          Norm.hpp
 * @author        zsh945
 * @date          2026-06-01
 */

#ifndef _NORM_
#define _NORM_

#include "Array.hpp"

using namespace std;

// Define norm types for error calculation
class Norm{
public:
    virtual double operator()(const Array& A) const = 0;
};

class L1Norm : public Norm {
public:
    double operator()(const Array& A) const {
        double sum = 0.0;
        for (int i = 0; i < A.size(); i++) {
            sum += abs(A[i]);
        }
        return sum;
    }
};

class L2Norm : public Norm {
public:
    double operator()(const Array& A) const {
        double sum = 0.0;
        for (int i = 0; i < A.size(); i++) {
            sum += A[i] * A[i];
        }
        return sqrt(sum);
    }
};

class LinfNorm : public Norm {
public:
    double operator()(const Array& A) const {
        double max_val = 0.0;
        for (int i = 0; i < A.size(); i++) {
            double val = abs(A[i]);
            if (val > max_val) { max_val = val; }
        }
        return max_val;
    }
};

#endif
