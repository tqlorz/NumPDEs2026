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
    GenerateMatrix_A(NodeCount, h, topologyInfo.circle());
    GenerateVector_F(NodeCount, h, u, topologyInfo.circle());
    SolveLinearEquation(NodeCount);
    cout << "Problem solved!" << endl;
    cout << "=============================================================" << endl;
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

/// @brief Generate the matrix A for the finite difference method
void Solver::GenerateMatrix_A(const int NodeCount, const double h, const Circle& circle) {
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
                GenerateMatrix_A_RectangleNeumann(k, i, j, h);
                break;
            }
            case CIRCLE_BOUNDARY_DIRICHLET: {
                GenerateMatrix_A_CircleDirichlet(k, i, j, h, circle);
                break;
            }
            case CIRCLE_BOUNDARY_NEUMANN: {
                GenerateMatrix_A_CircleNeumann(k, i, j, h, circle);
                break;
            }
            default: break;
        }
    }
}

/// @brief Generate the vector F for the finite difference method
/// @param u 
void Solver::GenerateVector_F(const int NodeCount, const double h, const BivariateFunction& u, const Circle& circle) {
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
                GenerateVector_F_RectangleNeumann(k, i, j, h, u);
                break;
            }
            case CIRCLE_BOUNDARY_DIRICHLET: {
                GenerateVector_F_CircleDirichlet(k, i, j, h, circle, u);
                break;
            }
            case CIRCLE_BOUNDARY_NEUMANN: {
                GenerateVector_F_CircleNeumann(k, i, j, h, circle, u);
                break;
            }
            default: break;
        }
    }
}

/// @brief Generate the matrix A for the rectangle with Neumann boundary conditions
/// @param k 
/// @param i 
/// @param j 
/// @param h 
void Solver::GenerateMatrix_A_RectangleNeumann(const int k, const int i, const int j, const double h) {
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
}

/// @brief Generate the matrix A for the circle with Dirichlet boundary conditions
/// @param k 
/// @param i 
/// @param j 
/// @param h 
/// @param circle 
void Solver::GenerateMatrix_A_CircleDirichlet(const int k, const int i, const int j, const double h, 
                                                const Circle& circle) {
    bool IsInCircle_Left, IsInCircle_Right, IsInCircle_Up, IsInCircle_Down;
    IsInCircle_Left = _BoundaryLabel(i - 1, j) == NOT_IN_CALCULATION;
    IsInCircle_Right = _BoundaryLabel(i + 1, j) == NOT_IN_CALCULATION;
    IsInCircle_Up = _BoundaryLabel(i, j + 1) == NOT_IN_CALCULATION;
    IsInCircle_Down = _BoundaryLabel(i, j - 1) == NOT_IN_CALCULATION;
    double alpha, theta;
    if (IsInCircle_Left == false && IsInCircle_Right == true) {
        alpha = (X_ON_CIRCLE_MINUS(j * h, circle.center_x(), circle.center_y(), circle.radius()) - i * h) / h;
    } else if (IsInCircle_Left == true && IsInCircle_Right == false) {
        alpha = (i * h - X_ON_CIRCLE_PLUS(j * h, circle.center_x(), circle.center_y(), circle.radius())) / h;
    } else {
        alpha = 1;
    }
    if (IsInCircle_Down == false && IsInCircle_Up == true) {
        theta = (Y_ON_CIRCLE_MINUS(i * h, circle.center_x(), circle.center_y(), circle.radius()) - j * h) / h;
    } else if (IsInCircle_Down == true && IsInCircle_Up == false) {
        theta = (j * h - Y_ON_CIRCLE_PLUS(i * h, circle.center_x(), circle.center_y(), circle.radius())) / h;
    } else {
        theta = 1;
    }
    _A(k, k) = 2 / (alpha * SQUARE(h)) + 2 / (theta * SQUARE(h));
    if (IsInCircle_Left == false) _A(k, _MeshToNodeOrder(i - 1, j)) = -alpha / (0.5 * alpha * (1 + alpha) * SQUARE(h));
    if (IsInCircle_Right == false) _A(k, _MeshToNodeOrder(i + 1, j)) = -alpha / (0.5 * alpha * (1 + alpha) * SQUARE(h));
    if (IsInCircle_Down == false) _A(k, _MeshToNodeOrder(i, j - 1)) = -theta / (0.5 * theta * (1 + theta) * SQUARE(h));
    if (IsInCircle_Up == false) _A(k, _MeshToNodeOrder(i, j + 1)) = -theta / (0.5 * theta * (1 + theta) * SQUARE(h));
}

/// @brief Generate the matrix A for the circle with Neumann boundary conditions
/// @param k 
/// @param i 
/// @param j 
/// @param h 
/// @param circle 
void Solver::GenerateMatrix_A_CircleNeumann(const int k, const int i, const int j, const double h, 
                                            const Circle& circle) {
    bool IsInCircle_Left, IsInCircle_Right, IsInCircle_Up, IsInCircle_Down;
    IsInCircle_Left = _BoundaryLabel(i - 1, j) == NOT_IN_CALCULATION;
    IsInCircle_Right = _BoundaryLabel(i + 1, j) == NOT_IN_CALCULATION;
    IsInCircle_Up = _BoundaryLabel(i, j + 1) == NOT_IN_CALCULATION;
    IsInCircle_Down = _BoundaryLabel(i, j - 1) == NOT_IN_CALCULATION;

    _A(k, k) = 4 / SQUARE(h);
    if (IsInCircle_Left == false) _A(k, _MeshToNodeOrder(i - 1, j)) = -1 / SQUARE(h);
    if (IsInCircle_Right == false) _A(k, _MeshToNodeOrder(i + 1, j)) = -1 / SQUARE(h);
    if (IsInCircle_Down == false) _A(k, _MeshToNodeOrder(i, j - 1)) = -1 / SQUARE(h);
    if (IsInCircle_Up == false) _A(k, _MeshToNodeOrder(i, j + 1)) = -1 / SQUARE(h);
    
    if (IsInCircle_Left == true) {
        double d = circle.radius() - DISTANCE_2D((i - 1) * h, j * h, circle.center_x(), circle.center_y());
        pair<double, double> NormalVector = make_pair((i - 1) * h - circle.center_x(), j * h - circle.center_y());
        pair<double, double> UnitNormalVector = make_pair(NormalVector.first / NORM_2D(NormalVector.first, NormalVector.second), 
                                                        NormalVector.second / NORM_2D(NormalVector.first, NormalVector.second));
        pair<double, double> PointOnCircle = make_pair((i - 1) * h + UnitNormalVector.first * d, 
                                                        j * h + UnitNormalVector.second * d);
        double distance = h / MAX(abs(NormalVector.first), abs(NormalVector.second));
        pair<double, double> PointOnExtendedLine = make_pair((i-1) * h + UnitNormalVector.first * distance, 
                                                        j * h + UnitNormalVector.second * distance);
        if (abs(NormalVector.first) >= abs(NormalVector.second)) {
            int index_x = round(PointOnExtendedLine.first / h);
            int index_y_up = ceil(PointOnExtendedLine.second / h);
            int index_y_down = floor(PointOnExtendedLine.second / h);
            _A(k, _MeshToNodeOrder(index_x, index_y_down)) += -(index_y_up - PointOnExtendedLine.second / h) / SQUARE(h);
            _A(k, _MeshToNodeOrder(index_x, index_y_up)) += -(PointOnExtendedLine.second / h - index_y_down) / SQUARE(h);
        } else {
            int index_y = round(PointOnExtendedLine.second / h);
            int index_x_left = floor(PointOnExtendedLine.first / h);
            int index_x_right = ceil(PointOnExtendedLine.first / h);
            _A(k, _MeshToNodeOrder(index_x_left, index_y)) += -(index_x_right - PointOnExtendedLine.first / h) / SQUARE(h);
            _A(k, _MeshToNodeOrder(index_x_right, index_y)) += -(PointOnExtendedLine.first / h - index_x_left) / SQUARE(h);
        }
    }
    if (IsInCircle_Right == true) {
        double d = circle.radius() - DISTANCE_2D((i + 1) * h, j * h, circle.center_x(), circle.center_y());
        pair<double, double> NormalVector = make_pair((i + 1) * h - circle.center_x(), j * h - circle.center_y());
        pair<double, double> UnitNormalVector = make_pair(NormalVector.first / NORM_2D(NormalVector.first, NormalVector.second), 
                                                        NormalVector.second / NORM_2D(NormalVector.first, NormalVector.second));
        pair<double, double> PointOnCircle = make_pair((i + 1) * h + UnitNormalVector.first * d, 
                                                        j * h + UnitNormalVector.second * d);
        double distance = h / MAX(abs(NormalVector.first), abs(NormalVector.second));
        pair<double, double> PointOnExtendedLine = make_pair((i+1) * h + UnitNormalVector.first * distance, 
                                                        j * h + UnitNormalVector.second * distance);
        if (abs(NormalVector.first) >= abs(NormalVector.second)) {
            int index_x = round(PointOnExtendedLine.first / h);
            int index_y_up = ceil(PointOnExtendedLine.second / h);
            int index_y_down = floor(PointOnExtendedLine.second / h);
            _A(k, _MeshToNodeOrder(index_x, index_y_down)) += -(index_y_up - PointOnExtendedLine.second / h) / SQUARE(h);
            _A(k, _MeshToNodeOrder(index_x, index_y_up)) += -(PointOnExtendedLine.second / h - index_y_down) / SQUARE(h);
        } else {
            int index_y = round(PointOnExtendedLine.second / h);
            int index_x_left = floor(PointOnExtendedLine.first / h);
            int index_x_right = ceil(PointOnExtendedLine.first / h);
            _A(k, _MeshToNodeOrder(index_x_left, index_y)) += -(index_x_right - PointOnExtendedLine.first / h) / SQUARE(h);
            _A(k, _MeshToNodeOrder(index_x_right, index_y)) += -(PointOnExtendedLine.first / h - index_x_left) / SQUARE(h);
        }
    }
    if (IsInCircle_Down == true) {
        double d = circle.radius() - DISTANCE_2D(i * h, (j - 1) * h, circle.center_x(), circle.center_y());
        pair<double, double> NormalVector = make_pair(i * h - circle.center_x(), (j - 1) * h - circle.center_y());
        pair<double, double> UnitNormalVector = make_pair(NormalVector.first / NORM_2D(NormalVector.first, NormalVector.second), 
                                                        NormalVector.second / NORM_2D(NormalVector.first, NormalVector.second));
        pair<double, double> PointOnCircle = make_pair(i * h + UnitNormalVector.first * d, 
                                                        (j - 1) * h + UnitNormalVector.second * d);
        double distance = h / MAX(abs(NormalVector.first), abs(NormalVector.second));
        pair<double, double> PointOnExtendedLine = make_pair(i * h + UnitNormalVector.first * distance, 
                                                        (j - 1) * h + UnitNormalVector.second * distance);
        if (abs(NormalVector.first) >= abs(NormalVector.second)) {
            int index_x = round(PointOnExtendedLine.first / h);
            int index_y_up = ceil(PointOnExtendedLine.second / h);
            int index_y_down = floor(PointOnExtendedLine.second / h);
            _A(k, _MeshToNodeOrder(index_x, index_y_down)) += -(index_y_up - PointOnExtendedLine.second / h) / SQUARE(h);
            _A(k, _MeshToNodeOrder(index_x, index_y_up)) += -(PointOnExtendedLine.second / h - index_y_down) / SQUARE(h);
        } else {
            int index_y = round(PointOnExtendedLine.second / h);
            int index_x_left = floor(PointOnExtendedLine.first / h);
            int index_x_right = ceil(PointOnExtendedLine.first / h);
            _A(k, _MeshToNodeOrder(index_x_left, index_y)) += -(index_x_right - PointOnExtendedLine.first / h) / SQUARE(h);
            _A(k, _MeshToNodeOrder(index_x_right, index_y)) += -(PointOnExtendedLine.first / h - index_x_left) / SQUARE(h);
        }
    }
    if (IsInCircle_Up == true) {
        double d = circle.radius() - DISTANCE_2D(i * h, (j + 1) * h, circle.center_x(), circle.center_y());
        pair<double, double> NormalVector = make_pair(i * h - circle.center_x(), (j + 1) * h - circle.center_y());
        pair<double, double> UnitNormalVector = make_pair(NormalVector.first / NORM_2D(NormalVector.first, NormalVector.second), 
                                                        NormalVector.second / NORM_2D(NormalVector.first, NormalVector.second));
        pair<double, double> PointOnCircle = make_pair(i * h + UnitNormalVector.first * d, 
                                                        (j + 1) * h + UnitNormalVector.second * d);
        double distance = h / MAX(abs(NormalVector.first), abs(NormalVector.second));
        pair<double, double> PointOnExtendedLine = make_pair(i * h + UnitNormalVector.first * distance, 
                                                        (j + 1) * h + UnitNormalVector.second * distance);
        if (abs(NormalVector.first) >= abs(NormalVector.second)) {
            int index_x = round(PointOnExtendedLine.first / h);
            int index_y_up = ceil(PointOnExtendedLine.second / h);
            int index_y_down = floor(PointOnExtendedLine.second / h);
            _A(k, _MeshToNodeOrder(index_x, index_y_down)) += -(index_y_up - PointOnExtendedLine.second / h) / SQUARE(h);
            _A(k, _MeshToNodeOrder(index_x, index_y_up)) += -(PointOnExtendedLine.second / h - index_y_down) / SQUARE(h);
        } else {
            int index_y = round(PointOnExtendedLine.second / h);
            int index_x_left = floor(PointOnExtendedLine.first / h);
            int index_x_right = ceil(PointOnExtendedLine.first / h);
            _A(k, _MeshToNodeOrder(index_x_left, index_y)) += -(index_x_right - PointOnExtendedLine.first / h) / SQUARE(h);
            _A(k, _MeshToNodeOrder(index_x_right, index_y)) += -(PointOnExtendedLine.first / h - index_x_left) / SQUARE(h);
        }
    }
}

/// @brief Generate the vector F for the rectangle with Neumann boundary conditions
/// @param k 
/// @param i 
/// @param j 
/// @param h 
/// @param u 
void Solver::GenerateVector_F_RectangleNeumann(const int k, const int i, const int j, const double h, 
                                            const BivariateFunction& u) {
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
}

/// @brief Generate the vector F for the circle with Dirichlet boundary conditions
/// @param k 
/// @param i 
/// @param j 
/// @param h 
/// @param circle 
/// @param u 
void Solver::GenerateVector_F_CircleDirichlet(const int k, const int i, const int j, const double h, 
                                            const Circle& circle, const BivariateFunction& u) {
    bool IsInCircle_Left, IsInCircle_Right, IsInCircle_Up, IsInCircle_Down;
    IsInCircle_Left = _BoundaryLabel(i - 1, j) == NOT_IN_CALCULATION;
    IsInCircle_Right = _BoundaryLabel(i + 1, j) == NOT_IN_CALCULATION;
    IsInCircle_Up = _BoundaryLabel(i, j + 1) == NOT_IN_CALCULATION;
    IsInCircle_Down = _BoundaryLabel(i, j - 1) == NOT_IN_CALCULATION;
    double alpha, theta;
    if (IsInCircle_Left == false && IsInCircle_Right == true) {
        alpha = (X_ON_CIRCLE_MINUS(j * h, circle.center_x(), circle.center_y(), circle.radius()) - i * h) / h;
    } else if (IsInCircle_Left == true && IsInCircle_Right == false) {
        alpha = (i * h - X_ON_CIRCLE_PLUS(j * h, circle.center_x(), circle.center_y(), circle.radius())) / h;
    } else {
        alpha = 1;
    }
    if (IsInCircle_Down == false && IsInCircle_Up == true) {
        theta = (Y_ON_CIRCLE_MINUS(i * h, circle.center_x(), circle.center_y(), circle.radius()) - j * h) / h;
    } else if (IsInCircle_Down == true && IsInCircle_Up == false) {
        theta = (j * h - Y_ON_CIRCLE_PLUS(i * h, circle.center_x(), circle.center_y(), circle.radius())) / h;
    } else {
        theta = 1;
    }
    _F(k) = (-1) * u.laplacian(i * h, j * h);
    if (IsInCircle_Left == true) _F(k) += u(X_ON_CIRCLE_PLUS(j * h, circle.center_x(), circle.center_y(), circle.radius()), j * h) / (0.5 * alpha * (1 + alpha) * SQUARE(h));
    if (IsInCircle_Right == true) _F(k) += u(X_ON_CIRCLE_MINUS(j * h, circle.center_x(), circle.center_y(), circle.radius()), j * h) / (0.5 * alpha * (1 + alpha) * SQUARE(h));
    if (IsInCircle_Down == true) _F(k) += u(i * h, Y_ON_CIRCLE_PLUS(i * h, circle.center_x(), circle.center_y(), circle.radius())) / (0.5 * theta * (1 + theta) * SQUARE(h));
    if (IsInCircle_Up == true) _F(k) += u(i * h, Y_ON_CIRCLE_MINUS(i * h, circle.center_x(), circle.center_y(), circle.radius())) / (0.5 * theta * (1 + theta) * SQUARE(h));
}

/// @brief Generate the vector F for the circle with Neumann boundary conditions
/// @param k 
/// @param i 
/// @param j 
/// @param h 
/// @param circle 
/// @param u 
void Solver::GenerateVector_F_CircleNeumann(const int k, const int i, const int j, const double h, 
                                            const Circle& circle, const BivariateFunction& u) {
    bool IsInCircle_Left, IsInCircle_Right, IsInCircle_Up, IsInCircle_Down;
    IsInCircle_Left = _BoundaryLabel(i - 1, j) == NOT_IN_CALCULATION;
    IsInCircle_Right = _BoundaryLabel(i + 1, j) == NOT_IN_CALCULATION;
    IsInCircle_Up = _BoundaryLabel(i, j + 1) == NOT_IN_CALCULATION;
    IsInCircle_Down = _BoundaryLabel(i, j - 1) == NOT_IN_CALCULATION;
    _F(k) = (-1) * u.laplacian(i * h, j * h);
    if (IsInCircle_Left == true) {
        double d = circle.radius() - DISTANCE_2D((i - 1) * h, j * h, circle.center_x(), circle.center_y());
        pair<double, double> NormalVector = make_pair((i - 1) * h - circle.center_x(), j * h - circle.center_y());
        pair<double, double> UnitNormalVector = make_pair(NormalVector.first / NORM_2D(NormalVector.first, NormalVector.second), 
                                                        NormalVector.second / NORM_2D(NormalVector.first, NormalVector.second));
        pair<double, double> PointOnCircle = make_pair((i - 1) * h + UnitNormalVector.first * d, 
                                                        j * h + UnitNormalVector.second * d);
        double distance = h / MAX(abs(NormalVector.first), abs(NormalVector.second));
        _F(k) += (u.partial_derivative_x(PointOnCircle.first, PointOnCircle.second) * (-UnitNormalVector.first) 
                + u.partial_derivative_y(PointOnCircle.first, PointOnCircle.second) * (-UnitNormalVector.second)) * distance / SQUARE(h);
    }
    if (IsInCircle_Right == true) {
        double d = circle.radius() - DISTANCE_2D((i + 1) * h, j * h, circle.center_x(), circle.center_y());
        pair<double, double> NormalVector = make_pair((i + 1) * h - circle.center_x(), j * h - circle.center_y());
        pair<double, double> UnitNormalVector = make_pair(NormalVector.first / NORM_2D(NormalVector.first, NormalVector.second), 
                                                        NormalVector.second / NORM_2D(NormalVector.first, NormalVector.second));
        pair<double, double> PointOnCircle = make_pair((i + 1) * h + UnitNormalVector.first * d, 
                                                        j * h + UnitNormalVector.second * d);
        double distance = h / MAX(abs(NormalVector.first), abs(NormalVector.second));
        _F(k) += (u.partial_derivative_x(PointOnCircle.first, PointOnCircle.second) * (-UnitNormalVector.first) 
                + u.partial_derivative_y(PointOnCircle.first, PointOnCircle.second) * (-UnitNormalVector.second)) * distance / SQUARE(h);
    }
    if (IsInCircle_Down == true) {
        double d = circle.radius() - DISTANCE_2D(i * h, (j - 1) * h, circle.center_x(), circle.center_y());
        pair<double, double> NormalVector = make_pair(i * h - circle.center_x(), (j - 1) * h - circle.center_y());
        pair<double, double> UnitNormalVector = make_pair(NormalVector.first / NORM_2D(NormalVector.first, NormalVector.second), 
                                                        NormalVector.second / NORM_2D(NormalVector.first, NormalVector.second));
        pair<double, double> PointOnCircle = make_pair(i * h + UnitNormalVector.first * d, 
                                                        (j - 1) * h + UnitNormalVector.second * d);
        double distance = h / MAX(abs(NormalVector.first), abs(NormalVector.second));
        _F(k) += (u.partial_derivative_x(PointOnCircle.first, PointOnCircle.second) * (-UnitNormalVector.first) 
                + u.partial_derivative_y(PointOnCircle.first, PointOnCircle.second) * (-UnitNormalVector.second)) * distance / SQUARE(h);
    }
    if (IsInCircle_Up == true) {
        double d = circle.radius() - DISTANCE_2D(i * h, (j + 1) * h, circle.center_x(), circle.center_y());
        pair<double, double> NormalVector = make_pair(i * h - circle.center_x(), (j + 1) * h - circle.center_y());
        pair<double, double> UnitNormalVector = make_pair(NormalVector.first / NORM_2D(NormalVector.first, NormalVector.second), 
                                                        NormalVector.second / NORM_2D(NormalVector.first, NormalVector.second));
        pair<double, double> PointOnCircle = make_pair(i * h + UnitNormalVector.first * d, 
                                                        (j + 1) * h + UnitNormalVector.second * d);
        double distance = h / MAX(abs(NormalVector.first), abs(NormalVector.second));
        _F(k) += (u.partial_derivative_x(PointOnCircle.first, PointOnCircle.second) * (-UnitNormalVector.first) 
                + u.partial_derivative_y(PointOnCircle.first, PointOnCircle.second) * (-UnitNormalVector.second)) * distance / SQUARE(h);
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