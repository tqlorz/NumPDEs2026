/**
 * @file          Solver.hpp
 * @author        zsh945
 * @date          2026-03-26
 * @brief         This file defines the Solver class, which is responsible for solving the problem using the specified algorithm.
 */

#ifndef _SOLVER_
#define _SOLVER_

#include "Shape.hpp"
#include "Tensor.hpp"
#include "MacroDef.hpp"
#include "Function.hpp"
#include "ResultAnalysis.hpp"
#include "TopologyInfo.hpp"
#include "Interpolation.hpp"
#include <lapacke.h>
#include <cmath>
#include <string>
#include <iostream>
#include <ostream>
#include <iomanip>

using namespace std;

/// @brief Class for solving the problem
class Solver {
private:
    Matrix_Int _ConnectedMesh, _MeshToNodeOrder, _BoundaryLabel;
    Tensor<pair<int, int>, 1> _NodeOrderToMesh;
    Matrix_Double _A;
    Vector_Double _F, _U;
    int NodeOrder();
    void BoundaryLabel(const int n, const Rectangle& rectangle, const Circle& circle, 
                    const vector<string>& rectangleBoundaryCondition, const string& circleBoundaryCondition);
    void GenerateMatrix_A(const int NodeCount, const double h, const Circle& circle);
    void GenerateVector_F(const int NodeCount, const double h, const BivariateFunction& u, const Circle& circle);
    void SolveLinearEquation(const int NodeCount);  
    // Auiliary function
    int GetRectangleBoundaryLabel(const int i, const int j, const Rectangle& rectangle, 
                                    const vector<string>& rectangleBoundaryCondition) const;
    int GetCircleBoundaryLabel(const int i, const int j, const double h, const Circle& circle, const string& circleBoundaryCondition) const;
    // Subfunctions for generating matrix A and vector F
    void GenerateMatrix_A_RectangleNeumann(const int k, const int i, const int j, const double h);
    void GenerateMatrix_A_CircleDirichlet(const int k, const int i, const int j, const double h, const Circle& circle);
    void GenerateMatrix_A_CircleNeumann(const int k, const int i, const int j, const double h, const Circle& circle);
    void GenerateVector_F_RectangleNeumann(const int k, const int i, const int j, const double h, const BivariateFunction& u);
    void GenerateVector_F_CircleNeumann(const int k, const int i, const int j, const double h, const Circle& circle, const BivariateFunction& u);
    void GenerateVector_F_CircleDirichlet(const int k, const int i, const int j, const double h, const Circle& circle, const BivariateFunction& u);
public:
    Solver() = default;
    Solver(const Matrix_Int& ConnectedMesh): _ConnectedMesh(ConnectedMesh)  {}
    void Solve(const TopologyInfo& topologyInfo, const BivariateFunction& u);
    void PrintResultFile(const TopologyInfo& topologyInfo, const BivariateFunction& u) const;
    const Matrix_Int& ConnectedMesh() const {return _ConnectedMesh;}
    const Matrix_Int& BoundaryLabel() const {return _BoundaryLabel;}
    const Matrix_Int& NodeOrderMesh() const {return _MeshToNodeOrder;}
    const Tensor<pair<int, int>, 1>& NodeOrderToMesh() const {return _NodeOrderToMesh;}
    const Matrix_Double& A() const {return _A;}
    const Vector_Double& F() const {return _F;}
    const Vector_Double& U() const {return _U;}
};

#endif