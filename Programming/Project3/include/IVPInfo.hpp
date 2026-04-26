/**
 * @file          IVPInfo.hpp
 * @author        zsh945
 * @date          2026-04-24
 */

#ifndef _IVPINFO_
#define _IVPINFO_

#include <vector>
#include <string>
#include "Array.hpp"
#include "Function.hpp"

using namespace std;

/// @brief Initial Value Problem (IVP) information structure
class IVPInfo{
private:
    string _MethodName;
    int _p;
    double _mu;
    double _TimeStep, _TotalTime;
    Array _InitialValue;
    // f(u, t, mu) is the function that defines the IVP, where u is the solution at time t, and mu is a parameter
    Function<Array, Array, double, double> _func;
public:
    IVPInfo() = default;
    IVPInfo(const string MethodName, const int p, const double mu, const double TimeStep, const double TotalTime, const Array &InitialValue, const Function<Array, Array, double, double> &func) : 
    _MethodName(MethodName), _p(p), _mu(mu), _TimeStep(TimeStep), _TotalTime(TotalTime), _InitialValue(InitialValue), _func(func) {}
    const string& MethodName() const {return _MethodName;}
    int p() const {return _p;}
    double mu() const {return _mu;}
    double TimeStep() const {return _TimeStep;}
    double TotalTime() const {return _TotalTime;}
    const Array& InitialValue() const {return _InitialValue;}
    const Function<Array, Array, double, double>& func() const {return _func;}
};

#endif