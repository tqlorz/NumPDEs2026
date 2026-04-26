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
    void PrintResultFile(const string& filename, const Array& u, const double t);
public:
    TimeIntegrator() = default;
    ~TimeIntegrator() = default;
    virtual void solve(const IVPInfo& IVPInfo) = 0;
};

/// @brief Print the result to file
/// @param filename 
/// @param u 
/// @param t 
void TimeIntegrator::PrintResultFile(const string& filename, const Array& u, const double t){
    if (t == 0) {
        ofstream initFile(filename, ios::trunc);
        initFile.close();
    }
    ofstream resultFile(filename, ios::app);
    if (!resultFile.is_open()) {
        cerr << "Failed to open the result file!" << endl;
        return;
    }
    resultFile << t << " ";
    for (size_t i = 0; i < u.size(); i++){
        resultFile << u[i];
        if (i != u.size() - 1) resultFile << " ";
    }
    resultFile << endl;
    resultFile.close();
}

#endif