/**
 * @file          MacroDef.hpp
 * @author        zsh945
 * @date          2026-03-26
 */

#ifndef _MACRODEF_
#define _MACRODEF_

#include <vector>
#include <string>

using namespace std;

// Define the macro for the ParameterCheck
#define INTERNAL_POINT 255
#define BOUNDARY_POINT 0

// Define the constant for the FileIO
const vector<string> BOUNDARY_CONDITION = {"Dirichlet", "Neumann"};

// Define some simplify calculation
#define SQUARE(x) ((x) * (x))
#define DISTANCE_2D(x1, y1, x2, y2) (sqrt(SQUARE((x2) - (x1)) + SQUARE((y2) - (y1))))

#endif