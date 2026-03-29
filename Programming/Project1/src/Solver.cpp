/**
 * @file          Solver.cpp
 * @author        zsh945
 * @date          2026-03-26
 */

#include "Solver.hpp"

/// @brief Solve the PDE using the finite difference method
void Solver::Solve(const TopologyInfo& topologyInfo, const BivariateFunction& u) {
    cout << "=============================================================" << endl;
    cout << "Solving the problem..." << endl;
    BoundaryLabel(topologyInfo.n(), topologyInfo.rectangle(), topologyInfo.circle(), 
                    topologyInfo.rectangle_boundary_condition(), topologyInfo.circle_boundary_condition());
    int NodeCount = NodeOrder();
    double h = max(topologyInfo.rectangle().width(), topologyInfo.rectangle().height()) / (topologyInfo.n() + 1);
    GenerateMatrix_A(NodeCount, h);
    GenerateVector_F(NodeCount, h, u);
    SolveLinearEquation(NodeCount);
    cout << "Problem solved!" << endl;
    cout << "=============================================================" << endl;
}

/// @brief Determine the node order for the finite difference method
int Solver::NodeOrder() {
    int NodeCount = 0;
    _MeshToNodeOrder.init(_BoundaryLabel.row(), _BoundaryLabel.col());
    cout << "Determining the node order..." << endl;
    for (int j = 0; j < _BoundaryLabel.col(); j++) {
        for (int i = 0; i < _BoundaryLabel.row(); i++) {
            if (_BoundaryLabel(i, j) != NOT_IN_CALCULATION) {
                _MeshToNodeOrder(i, j) = NodeCount;
                NodeCount++;
            } else {
                _MeshToNodeOrder(i, j) = -1;
            }
        }
    }
    _NodeOrderToMesh.init(NodeCount);
    for (int j = 0; j < _MeshToNodeOrder.col(); j++) {
        for (int i = 0; i < _MeshToNodeOrder.row(); i++) {
            if (_MeshToNodeOrder(i, j) != -1) {
                _NodeOrderToMesh(_MeshToNodeOrder(i, j)) = make_pair(i, j);
            }
        }
    }
    return NodeCount;
}

/// @brief Label the boundary points for the finite difference method
void Solver::BoundaryLabel(const int n, const Rectangle& rectangle, const Circle& circle, 
                    const vector<string>& rectangleBoundaryCondition, const string& circleBoundaryCondition) {
    cout << "Labeling the boundary points..." << endl;
    _BoundaryLabel.init(_ConnectedMesh.row(), _ConnectedMesh.col());
    int CenterData;
    double h = max(rectangle.width(), rectangle.height()) / (n + 1);
    for (int i = 0; i < _ConnectedMesh.row(); i++) {
        for (int j = 0; j < _ConnectedMesh.col(); j++) {
            CenterData = (_ConnectedMesh(i, j) == 1) ? INTERNAL_POINT : BOUNDARY_POINT;
            // Briefly Label the calculation region
            if (CenterData == INTERNAL_POINT) {
                _BoundaryLabel(i, j) = NOT_BOUNDARY;
            } else if (CenterData == BOUNDARY_POINT) {
                _BoundaryLabel(i, j) = NOT_IN_CALCULATION;
            }
            // Label the boundary points
            if (_BoundaryLabel(i, j) == NOT_IN_CALCULATION) {
                _BoundaryLabel(i, j) = GetRectangleBoundaryLabel(i, j, rectangle, rectangleBoundaryCondition);
            } else if (_BoundaryLabel(i, j) == NOT_BOUNDARY) {
                _BoundaryLabel(i, j) = GetCircleBoundaryLabel(i, j, h, circle, circleBoundaryCondition);
            }
        }
    }
    // address the corner point of rectangle
    _BoundaryLabel(0, 0) = NOT_IN_CALCULATION;
    _BoundaryLabel(0, _ConnectedMesh.col() - 1) = NOT_IN_CALCULATION;
    _BoundaryLabel(_ConnectedMesh.row() - 1, 0) = NOT_IN_CALCULATION;
    _BoundaryLabel(_ConnectedMesh.row() - 1, _ConnectedMesh.col() - 1) = NOT_IN_CALCULATION;    
}

/// @brief Get the boundary label for a rectangle point
/// @param i 
/// @param j 
/// @param rectangle 
/// @param rectangleBoundaryCondition 
int Solver::GetRectangleBoundaryLabel(const int i, const int j, const Rectangle& rectangle, 
                                    const vector<string>& rectangleBoundaryCondition) const {
    string rectangleBoundaryCondition_Left = rectangleBoundaryCondition[0];
    string rectangleBoundaryCondition_Right = rectangleBoundaryCondition[1];
    string rectangleBoundaryCondition_Up = rectangleBoundaryCondition[2];
    string rectangleBoundaryCondition_Down = rectangleBoundaryCondition[3];
    if (i == 0) {
        return (rectangleBoundaryCondition_Left == "Dirichlet") ? 
                RECTANGLE_BOUNDARY_DIRICHLET : RECTANGLE_BOUNDARY_NEUMANN;
    }
    if (i == _ConnectedMesh.row() - 1) {
        return (rectangleBoundaryCondition_Right == "Dirichlet") ? 
                RECTANGLE_BOUNDARY_DIRICHLET : RECTANGLE_BOUNDARY_NEUMANN;
               
    }
    if (j == 0) {
        return (rectangleBoundaryCondition_Down == "Dirichlet") ? 
                RECTANGLE_BOUNDARY_DIRICHLET : RECTANGLE_BOUNDARY_NEUMANN;
    }
    if (j == _ConnectedMesh.col() - 1) {
        return (rectangleBoundaryCondition_Up == "Dirichlet") ? 
                RECTANGLE_BOUNDARY_DIRICHLET : RECTANGLE_BOUNDARY_NEUMANN;
    }
    return NOT_IN_CALCULATION;
}

/// @brief Get the boundary label for a circle point
/// @param i 
/// @param j 
/// @param circle 
/// @param circleBoundaryCondition 
int Solver::GetCircleBoundaryLabel(const int i, const int j, const double h,
                                    const Circle& circle, const string& circleBoundaryCondition) const {
    if (IS_IN_CIRCLE((i - 1) * h, j * h, circle.center_x(), circle.center_y(), circle.radius()) ||
        IS_IN_CIRCLE((i + 1) * h, j * h, circle.center_x(), circle.center_y(), circle.radius()) ||
        IS_IN_CIRCLE(i * h, (j + 1) * h, circle.center_x(), circle.center_y(), circle.radius()) ||
        IS_IN_CIRCLE(i * h, (j - 1) * h, circle.center_x(), circle.center_y(), circle.radius())) {
        return (circleBoundaryCondition == "Dirichlet") ? 
                CIRCLE_BOUNDARY_DIRICHLET : CIRCLE_BOUNDARY_NEUMANN;
    }
    return NOT_BOUNDARY;
}

/// @brief Generate the matrix A for the finite difference method
void Solver::GenerateMatrix_A(const int NodeCount, const double h) {
    cout << "Generating the matrix A..." << endl;
    _A.init(NodeCount, NodeCount);
    // this may be optimized (Dirichlet?)
    for (int k = 0; k < NodeCount; k++) {
        int i = _NodeOrderToMesh(k).first;
        int j = _NodeOrderToMesh(k).second;
        switch (_BoundaryLabel(i, j)) {
            case NOT_BOUNDARY: {
                _A(k, k) = 4 / SQUARE(h);
                _A(k, _MeshToNodeOrder(i - 1, j)) = -1 / SQUARE(h);
                _A(k, _MeshToNodeOrder(i + 1, j)) = -1 / SQUARE(h);
                _A(k, _MeshToNodeOrder(i, j - 1)) = -1 / SQUARE(h);
                _A(k, _MeshToNodeOrder(i, j + 1)) = -1 / SQUARE(h);
                break;
            }
            case RECTANGLE_BOUNDARY_DIRICHLET: {
                _A(k, k) = 1;
                break;
            }
            case RECTANGLE_BOUNDARY_NEUMANN: {
                if (i == 0) {
                    _A(k, k) = 4 / SQUARE(h);
                    _A(k, _MeshToNodeOrder(1, j)) = -2 / SQUARE(h);
                    if (j - 1 > 0) _A(k, _MeshToNodeOrder(0, j - 1)) = -1 / SQUARE(h);
                    if (j + 1 < _ConnectedMesh.col() - 1) _A(k, _MeshToNodeOrder(0, j + 1)) = -1 / SQUARE(h);
                } else if (i == _ConnectedMesh.row() - 1) {
                    _A(k, k) = 4 / SQUARE(h);
                    _A(k, _MeshToNodeOrder(_ConnectedMesh.row() - 2, j)) = -2 / SQUARE(h);
                    if (j - 1 > 0) _A(k, _MeshToNodeOrder(_ConnectedMesh.row() - 1, j - 1)) = -1 / SQUARE(h);
                    if (j + 1 < _ConnectedMesh.col() - 1) _A(k, _MeshToNodeOrder(_ConnectedMesh.row() - 1, j + 1)) = -1 / SQUARE(h);
                } else if (j == 0) {
                    _A(k, k) = 4 / SQUARE(h);
                    _A(k, _MeshToNodeOrder(i, 1)) = -2 / SQUARE(h);
                    if (i - 1 > 0) _A(k, _MeshToNodeOrder(i - 1, 0)) = -1 / SQUARE(h);
                    if (i + 1 < _ConnectedMesh.row() - 1) _A(k, _MeshToNodeOrder(i + 1, 0)) = -1 / SQUARE(h);
                } else if (j == _ConnectedMesh.col() - 1) {
                    _A(k, k) = 4 / SQUARE(h);
                    _A(k, _MeshToNodeOrder(i, _ConnectedMesh.col() - 2)) = -2 / SQUARE(h);
                    if (i - 1 > 0) _A(k, _MeshToNodeOrder(i - 1, _ConnectedMesh.col() - 1)) = -1 / SQUARE(h);
                    if (i + 1 < _ConnectedMesh.row() - 1) _A(k, _MeshToNodeOrder(i + 1, _ConnectedMesh.col() - 1)) = -1 / SQUARE(h);
                }
                break;
            }
            case CIRCLE_BOUNDARY_DIRICHLET: break;
            case CIRCLE_BOUNDARY_NEUMANN: break;
            case NOT_IN_CALCULATION: break;
            default: break;
        }
    }
}

/// @brief Generate the vector F for the finite difference method
/// @param u 
void Solver::GenerateVector_F(const int NodeCount, const double h, const BivariateFunction& u) {
    cout << "Generating the vector F..." << endl;
    _F.init(NodeCount);
    // this may be optimized (Dirichlet?)
    for (int k = 0; k < NodeCount; k++) {
        int i = _NodeOrderToMesh(k).first;
        int j = _NodeOrderToMesh(k).second;
        switch (_BoundaryLabel(i, j)) {
            case NOT_BOUNDARY: {
                _F(k) = (-1) * u.laplacian(i * h, j * h);
                break;
            }
            case RECTANGLE_BOUNDARY_DIRICHLET: {
                _F(k) = u(i * h, j * h);
                break;
            }
            case RECTANGLE_BOUNDARY_NEUMANN: {
                if (i == 0) {
                    _F(k) = (-1) * u.laplacian(0, j * h) + (2 / h) * (-u.partial_derivative_x(0, j * h));
                    if (j - 1 == 0) _F(k) += u(0, 0) / SQUARE(h);
                    if (j + 1 == _ConnectedMesh.col() - 1) _F(k) += u(0, (_ConnectedMesh.col() - 1) * h) / SQUARE(h);
                } else if (i == _ConnectedMesh.row() - 1) {
                    _F(k) = (-1) * u.laplacian((_ConnectedMesh.row() - 1) * h, j * h) + (2 / h) * u.partial_derivative_x((_ConnectedMesh.row() - 1) * h, j * h);
                    if (j - 1 == 0) _F(k) += u((_ConnectedMesh.row() - 1) * h, 0) / SQUARE(h);
                    if (j + 1 == _ConnectedMesh.col() - 1) _F(k) += u((_ConnectedMesh.row() - 1) * h, (_ConnectedMesh.col() - 1) * h) / SQUARE(h);
                } else if (j == 0) {
                    _F(k) = (-1) * u.laplacian(i * h, 0) + (2 / h) * (-u.partial_derivative_y(i * h, 0));
                    if (i - 1 == 0) _F(k) += u(0, 0) / SQUARE(h);
                    if (i + 1 == _ConnectedMesh.row() - 1) _F(k) += u((_ConnectedMesh.row() - 1) * h, 0) / SQUARE(h);
                } else if (j == _ConnectedMesh.col() - 1) {
                    _F(k) = (-1) * u.laplacian(i * h, (_ConnectedMesh.col() - 1) * h) + (2 / h) * u.partial_derivative_y(i * h, (_ConnectedMesh.col() - 1) * h);
                    if (i - 1 == 0) _F(k) += u(0, (_ConnectedMesh.col() - 1) * h) / SQUARE(h);
                    if (i + 1 == _ConnectedMesh.row() - 1) _F(k) += u((_ConnectedMesh.row() - 1) * h, (_ConnectedMesh.col() - 1) * h) / SQUARE(h);
                }
                break;
            }
            case CIRCLE_BOUNDARY_DIRICHLET: {
                _F(k) = u(i * h, j * h);
                break;
            }
            case CIRCLE_BOUNDARY_NEUMANN: break;
            case NOT_IN_CALCULATION: break;
            default: break;
        }
    }
}

/// @brief Solve the linear equation AU = F
void Solver::SolveLinearEquation(const int NodeCount) {
    _U.init(NodeCount);
    _U = _F;
    Matrix_Int ipiv(_A.row(), _A.row());
    LAPACKE_dgesv(LAPACK_ROW_MAJOR, NodeCount, 1, _A.data(), NodeCount, ipiv.data(), _U.data(), 1);
}

/// @brief Print the result of the solution to the file
/// @param topologyInfo 
/// @param u 
void Solver::PrintResultFile(const TopologyInfo& topologyInfo, const BivariateFunction& u) const {
    cout << "Printing the result to the file..." << endl;
    ofstream resultFile("output/data/result.txt");
    if (!resultFile.is_open()) {
        cerr << "Failed to open the result file!" << endl;
        return;
    }
    double h = max(topologyInfo.rectangle().width(), topologyInfo.rectangle().height()) / (topologyInfo.n() + 1);
    for (int k = 0; k < _U.size(); k++) {
        int i = _NodeOrderToMesh(k).first;
        int j = _NodeOrderToMesh(k).second;
        resultFile << setprecision(10) << i * h << " " << j * h << " " << _U(k) << " " << u(i * h, j * h) << endl;
    }
    resultFile.close();
}