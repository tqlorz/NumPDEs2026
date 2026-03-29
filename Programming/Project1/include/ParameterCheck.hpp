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
#include "TopologyInfo.hpp"
#include <iostream>
#include <string>

using namespace std;

class ParameterCheck {
private:
    Matrix_Int _Mesh, _ConnectedMesh;
    void DataCheck(const int n, const Rectangle& rectangle, const Circle& circle, 
                    const vector<string>& rectangleBoundaryCondition, const string& circleBoundaryCondition) const;
    void GenerateMesh(const int n, const Rectangle& rectangle, const Circle& circle);
    void MeshCheck(const int n, const Rectangle& rectangle, const Circle& circle);
public:
    ParameterCheck() = default;
    const Matrix_Int& Mesh() const {return _Mesh;}
    const Matrix_Int& ConnectedMesh() const {return _ConnectedMesh;}
    void Check(const TopologyInfo& topologyInfo);
};


#endif