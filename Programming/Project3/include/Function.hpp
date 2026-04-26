/**
 * @file          Function.hpp
 * @author        zsh945
 * @date          2026-04-24
 * @brief         This file defines the base class of function
 */
#ifndef _FUNCTION_
#define _FUNCTION_

#include <cmath>
#include <vector>
#include <functional>
#include "Array.hpp"

using namespace std;

/// @brief Muti-variable function class
/// @tparam T 
/// @tparam ...Args 
template<typename T, typename... Args>
class Function {
private:
    std::function<T(Args...)> _func;
public:
    template<typename F>
    Function(F&& f) : _func(std::forward<F>(f)) {}
    T operator()(Args... args) const {
        return _func(std::forward<Args>(args)...);
    }
};
// Define function for the planar motion
inline Function<double, Array, double, double> 
                f_1([](const Array& u, const double t, double mu) { 
                    return u[4-1]; 
                });
inline Function<double, Array, double, double> 
                f_2([](const Array& u, const double t, double mu) { 
                    return u[5-1]; 
                });
inline Function<double, Array, double, double> 
                f_3([](const Array& u, const double t, double mu) { 
                    return u[6-1]; 
                });
inline Function<double, Array, double, double> 
                f_4([](const Array& u, const double t, double mu) { 
                    return 2*u[5-1] + u[1-1]
                            - mu*(u[1-1]+mu-1) / pow(pow(u[2-1], 2) + pow(u[3-1], 2) + pow((u[1-1]+mu-1), 2), 3.0/2.0) 
                            - (1-mu)*(u[1-1]+mu) / pow(pow(u[2-1], 2) + pow(u[3-1], 2) + pow((u[1-1]+mu), 2), 3.0/2.0);
                });
inline Function<double, Array, double, double> 
                f_5([](const Array& u, const double t, double mu) { 
                    return -2*u[4-1] + u[2-1]
                            - mu*u[2-1] / pow(pow(u[2-1], 2) + pow(u[3-1], 2) + pow((u[1-1]+mu-1), 2), 3.0/2.0) 
                            - (1-mu)*u[2-1] / pow(pow(u[2-1], 2) + pow(u[3-1], 2) + pow((u[1-1]+mu), 2), 3.0/2.0);
                });
inline Function<double, Array, double, double> 
                f_6([](const Array& u, const double t, double mu) { 
                    return - mu*u[3-1] / pow(pow(u[2-1], 2) + pow(u[3-1], 2) + pow((u[1-1]+mu-1), 2), 3.0/2.0) 
                            - (1-mu)*u[3-1] / pow(pow(u[2-1], 2) + pow(u[3-1], 2) + pow((u[1-1]+mu), 2), 3.0/2.0);
                });

inline Function<Array, Array, double, double> 
                Example_func([](const Array& u, const double t, double mu) { 
                    return Array{f_1(u, t, mu), f_2(u, t, mu), f_3(u, t, mu), 
                                            f_4(u, t, mu), f_5(u, t, mu), f_6(u, t, mu)};
                });

#endif