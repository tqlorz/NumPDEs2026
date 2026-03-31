/**
 * @file          Interpolation.hpp
 * @author        zsh945
 * @date          2026-03-30
 */

#ifndef _INTERPOLATION_
#define _INTERPOLATION_

#include "Shape.hpp"
#include "MacroDef.hpp"
#include <vector>
#include <cmath>

using namespace std;

vector<double> LinearInterpolation(const double Point_x, const double LeftPoint_x, const double RightPoint_x);

vector<double> LinearInterpolation(const double Point_x, const double Point_y,
                                    const double LeftPoint_x, const double LeftPoint_y,
                                    const double RightPoint_x, const double RightPoint_y);

vector<double> RectangleInterpolation(const double Point_x, const double Point_y,
                                    const Rectangle& rectangle);

vector<double> TriangleInterpolation(const double Point_x, const double Point_y,
                                    const Triangle& triangle);

#endif