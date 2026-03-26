/**
 * @file          Solver.hpp
 * @author        zsh945
 * @date          2026-03-26
 * @brief         This file defines the Solver class, which is responsible for solving the problem using the specified algorithm.
 */

#ifndef _SOLVER_
#define _SOLVER_

#include "Shape.hpp"
#include <string>

using namespace std;

class Solver {
private:
    int _n;
    Rectangle _Rectangle;
    Circle _Circle;
    string _RectangleBoundaryCondition, _CircleBoundaryCondition;
public:
    Solver(): _n(0), _Rectangle(0, 0, 0, 0), _Circle(0, 0, 0), 
                _RectangleBoundaryCondition(""), _CircleBoundaryCondition("") {};
};

#endif