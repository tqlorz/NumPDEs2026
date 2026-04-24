/**
 * @file          Function.hpp
 * @author        zsh945
 * @date          2026-04-24
 * @brief         This file defines the base class of bivariate function
 */
#ifndef _FUNCTION_
#define _FUNCTION_

#include <vector>

using namespace std;

/// @brief base class of bivariate function
class BivariateFunction {
public:
    /// @brief pure virtual function to calculate the value of the function
    /// @param x 
    /// @param y
    /// @return f(x, y)
    virtual double operator()(const double x, const double y) const = 0;
    virtual double partial_derivative_x(const double x, const double y, const double delta_x = 1e-6) const {
        return (operator()(x + delta_x, y) - operator()(x - delta_x, y)) / (2 * delta_x);
    }
    virtual double partial_derivative_y(const double x, const double y, const double delta_y = 1e-6) const {
        return (operator()(x, y + delta_y) - operator()(x, y - delta_y)) / (2 * delta_y);
    }
    virtual double partial_derivative_xx(const double x, const double y, const double delta_x = 1e-6) const {
        return (operator()(x + delta_x, y) - 2 * operator()(x, y) + operator()(x - delta_x, y)) / (delta_x * delta_x);
    }
    virtual double partial_derivative_yy(const double x, const double y, const double delta_y = 1e-6) const {
        return (operator()(x, y + delta_y) - 2 * operator()(x, y) + operator()(x, y - delta_y)) / (delta_y * delta_y);
    }
    virtual double laplacian(const double x, const double y, const double delta_x = 1e-6, const double delta_y = 1e-6) const {
        return partial_derivative_xx(x, y, delta_x) + partial_derivative_yy(x, y, delta_y);
    }
};

#endif