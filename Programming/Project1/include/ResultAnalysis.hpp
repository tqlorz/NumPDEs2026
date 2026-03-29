/**
 * @file          ResultAnalysis.hpp
 * @author        zsh945
 * @date          2026-03-26
 */

#ifndef _RESULTANALYSIS_
#define _RESULTANALYSIS_

#include "Tensor.hpp"
#include "MacroDef.hpp"

template<typename T, int order>
class Norm{
public:
    virtual double operator()(const Tensor<T, order>& A) const = 0;
};

template<typename T, int order>
class L1Norm : public Norm<T, order> {
public:
    double operator()(const Tensor<T, 1>& A) const {
        double h = 1.0 / (A.size() - 1);
        double sum = 0.0;
        for (int i = 0; i < A.size(); i++) {
            sum += abs(A(i));
        }
        return h * sum;
    }
};

template<typename T, int order>
class L2Norm : public Norm<T, order> {
public:
    double operator()(const Tensor<T, 1>& A) const {
        double h = 1.0 / (A.size() - 1);
        double sum = 0.0;
        for (int i = 0; i < A.size(); i++) {
            sum += SQUARE(A(i));
        }
        return sqrt(h * sum);
    }
};

template<typename T, int order>
class LinfNorm : public Norm<T, order> {
public:
    double operator()(const Tensor<T, 1>& A) const {
        double max_val = 0.0;
        for (int i = 0; i < A.size(); i++) {
            double val = abs(A(i));
            if (val > max_val) { max_val = val; }
        }
        return max_val;
    }
};

#endif