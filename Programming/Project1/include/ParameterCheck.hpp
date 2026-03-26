/**
 * @file          ParameterCheck.hpp
 * @author        zsh945
 * @date          2026-03-26
 * @brief         This file defines the ParameterCheck class, 
 *                which is responsible for data from json file.
 */

#ifndef _PARAMETERCHECK_
#define _PARAMETERCHECK_

#include "Tensor.hpp"
#include "Shape.hpp"
#include "MacroDef.hpp"
#include "ConnectedRegion.hpp"
#include <iostream>
#include <string>

using namespace std;

class ParameterCheck {
private:
    int _n;
    double _h;
    Rectangle _Rectangle;
    Circle _Circle;
    string _RectangleBoundaryCondition, _CircleBoundaryCondition;
    Matrix_Int _Mesh;
    void DataCheck() const;
    void GenerateMesh();
    void MeshCheck() const;
public:
    ParameterCheck(): _n(0), _Rectangle(0, 0, 0, 0), _Circle(0, 0, 0), _RectangleBoundaryCondition(""), _CircleBoundaryCondition("") {};
    ParameterCheck(const int n, const Rectangle& rectangle, const Circle& circle, 
                    const string& rectangleBoundaryCondition, const string& circleBoundaryCondition): 
                    _n(n), _Rectangle(rectangle), _Circle(circle), 
                    _RectangleBoundaryCondition(rectangleBoundaryCondition), _CircleBoundaryCondition(circleBoundaryCondition) {};
    Matrix_Int Mesh() const {return _Mesh;}
    void Check();
};


#endif