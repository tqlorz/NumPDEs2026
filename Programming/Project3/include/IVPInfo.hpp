/**
 * @file          IVPInfo.hpp
 * @author        zsh945
 * @date          2026-04-24
 */

#ifndef _IVPINFO_
#define _IVPINFO_

#include <vector>
#include <string>
#include "Tensor.hpp"

using namespace std;

class IVPInfo{
private:
    string _MethodName;
    int _p;
    double _mu;
    double _TimeStep, _TotalTime;
    Vector_Double _InitialValue;
public:
    IVPInfo() = default;
    IVPInfo(const string MethodName, const int p, const double mu, const double TimeStep, const double TotalTime, const Vector_Double &InitialValue) : 
    _MethodName(MethodName), _p(p), _mu(mu), _TimeStep(TimeStep), _TotalTime(TotalTime), _InitialValue(InitialValue) {}
    const string& MethodName() const {return _MethodName;}
    int p() const {return _p;}
    double mu() const {return _mu;}
    double TimeStep() const {return _TimeStep;}
    double TotalTime() const {return _TotalTime;}
    const Vector_Double& InitialValue() const {return _InitialValue;}
};

#endif