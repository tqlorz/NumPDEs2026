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

// Define the macro for the boundary label
#define NOT_IN_CALCULATION -1
#define NOT_BOUNDARY 0
#define RECTANGLE_BOUNDARY_DIRICHLET 1
#define RECTANGLE_BOUNDARY_NEUMANN 2
#define CIRCLE_BOUNDARY_DIRICHLET 3
#define CIRCLE_BOUNDARY_NEUMANN 4

// Define some simplify calculation
#define SQUARE(x) ((x) * (x))
#define NORM_2D(x, y) (sqrt(SQUARE(x) + SQUARE(y)))
#define DISTANCE_2D(x1, y1, x2, y2) (sqrt(SQUARE((x2) - (x1)) + SQUARE((y2) - (y1))))
#define IS_IN_CIRCLE(x, y, x_0, y_0, r) (SQUARE((x) - (x_0)) + SQUARE((y) - (y_0)) <= SQUARE(r))
#define X_ON_CIRCLE_MINUS(y, x_0, y_0, r) ((x_0) - sqrt(SQUARE(r) - SQUARE((y) - (y_0))))
#define X_ON_CIRCLE_PLUS(y, x_0, y_0, r) ((x_0) + sqrt(SQUARE(r) - SQUARE((y) - (y_0))))
#define Y_ON_CIRCLE_MINUS(x, x_0, y_0, r) ((y_0) - sqrt(SQUARE(r) - SQUARE((x) - (x_0))))
#define Y_ON_CIRCLE_PLUS(x, x_0, y_0, r) ((y_0) + sqrt(SQUARE(r) - SQUARE((x) - (x_0))))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#endif