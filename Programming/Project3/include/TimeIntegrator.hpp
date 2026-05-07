/**
 * @file          TimeIntegrator.hpp
 * @author        zsh945
 * @date          2026-04-24
 * @brief         TimeIntegrator class declaration
 */

#ifndef _TIMEINTEGRATOR_
#define _TIMEINTEGRATOR_

#include <iostream>
#include <fstream>
#include <cmath>
#include "IVPInfo.hpp"
#include "Function.hpp"
#include "Array.hpp"
#include "MacroDef.hpp"

using namespace std;

/// @brief The base class for time integrators
class TimeIntegrator {
protected:
    Array u_end;
    void PrintResultFile(const string& filename, const Array& u, const double t);
public:
    TimeIntegrator() = default;
    ~TimeIntegrator() = default;
    virtual void solve(const IVPInfo& IVPInfo) = 0;
    Array solution() const { return u_end; }
};

#endif