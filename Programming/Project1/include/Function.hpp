/**
 * @file          Function.hpp
 * @author        zsh945
 * @date          2026-03-26
 * @brief         This file defines the base class of bivariate function.
 */

#ifndef _FUNCTION_
#define _FUNCTION_

/// @brief base class of bivariate function
class BivariateFunction {
public:
    /// @brief pure virtual function to calculate the value of the function
    /// @param x 
    /// @param y
    /// @return f(x, y)
    virtual double operator()(const double x, const double y) const = 0;
};

#endif