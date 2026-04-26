/**
 * @file          MacroDef.hpp
 * @author        zsh945
 * @date          2026-04-24
 */

#ifndef _MACRODEF_
#define _MACRODEF_

#include <vector>

using namespace std;

// Define coefficients for linear multistep methods
struct Coefficients_LMM {
    vector<double> alpha;
    vector<double> beta;
    int p2s;
};
typedef struct Coefficients_LMM Coefficients_LMM;
// Coefficients for Adams-Bashforth methods of p = 1-4
const vector<Coefficients_LMM> Coefficients_ABMs = {
    { {-1.0, 1.0}, {1.0, 0}, 1 },
    { {0, -1.0, 1.0}, {-1.0/2, 3.0/2, 0}, 2 },
    { {0, 0, -1.0, 1.0}, {5.0/12, -16.0/12, 23.0/12, 0}, 3 },
    { {0, 0, 0, -1.0, 1.0}, {-9.0/24, 37.0/24, -59.0/24, 55.0/24, 0}, 4 }
};
// Coefficients for Adams-Moulton methods of p = 1-5
const vector<Coefficients_LMM> Coefficients_AMMs = {
    { {-1.0, 1.0}, {0, 1.0}, 1 },
    { {-1.0, 1.0}, {1.0/2, 1.0/2}, 1 },
    { {0, -1.0, 1.0}, {-1.0/12, 8.0/12, 5.0/12}, 2 },
    { {0, 0, -1.0, 1.0}, {1.0/24, -5.0/24, 19.0/24, 9.0/24}, 3 },
    { {0, 0, 0, -1.0, 1.0}, {-19.0/720, 106.0/720, -264.0/720, 646.0/720, 251.0/720}, 4 }
};
// Coefficients for BDFs methods of p = 1-4
const vector<Coefficients_LMM> Coefficients_BDFs = {
    { {-1.0, 1.0}, {1.0, 0}, 1 },
    { {1.0/3, -4.0/3, 1.0}, {0, 0, 2.0/3}, 2 },
    { {-2.0/11, 9.0/11, -18.0/11, 1.0}, {0, 0, 0, 6.0/11}, 3 },
    { {3.0/25, -16.0/25, 36.0/25, -48.0/25, 1.0}, {0, 0, 0, 0, 12.0/25}, 4 }
};

// Define coefficients for linear multistep methods
struct Coefficients_RK {
    vector<vector<double>> a;
    vector<double> b;
    vector<double> c;
    int p2s;
};
typedef struct Coefficients_RK Coefficients_RK;
// Coefficients for classical Runge-Kutta methods of p = 1-4
const vector<Coefficients_RK> Coefficients_classicalRK = {
    { {{0}}, {1.0}, {0}, 1 },
    { {{0, 0}, 
       {1.0, 0}}, {1.0/2, 1.0/2}, {0, 1.0}, 2 },
    { {{0, 0, 0}, 
       {1.0/3, 0, 0}, 
       {0, 2.0/3, 0}}, {1.0/4, 0, 3.0/4}, {0, 1.0/3, 2.0/3}, 3 },
    { {{0, 0, 0, 0}, 
       {1.0/2, 0, 0, 0}, 
       {0, 1.0/2, 0, 0}, 
       {0, 0, 1.0, 0}}, {1.0/6, 2.0/6, 2.0/6, 1.0/6}, {0, 1.0/2, 1.0/2, 1.0}, 4 }
};

#endif