/**
 * @file          ParameterCheck.cpp
 * @author        zsh945
 * @date          2026-03-26
 */

#include "ParameterCheck.hpp"

/// @brief Generate the mesh for the ParameterCheck class. 
void ParameterCheck::GenerateMesh(int n, const Rectangle& rectangle, const Circle& circle) {
    cout << "Generating mesh for check..." << endl;
    double h = max(rectangle.width(), rectangle.height()) / (n + 1);
    _Mesh.init(n + 2, n + 2);
    for (int i = 0; i < _Mesh.row(); i++) {
        for (int j = 0; j < _Mesh.col(); j++) {
            if (IS_IN_CIRCLE(i * h, j * h, circle.center_x(), circle.center_y(), circle.radius())) {
                _Mesh(i, j) = BOUNDARY_POINT;
            } else if (i == 0 || i == _Mesh.row() - 1 || j == 0 || j == _Mesh.col() - 1) {
                _Mesh(i, j) = BOUNDARY_POINT;
            } else {
                _Mesh(i, j) = INTERNAL_POINT;
            }
        }
    }
}

/// @brief Check the data from json file
void ParameterCheck::DataCheck(const int n, const Rectangle& rectangle, const Circle& circle, 
                    const vector<string>& rectangleBoundaryCondition, const string& circleBoundaryCondition) const {
    if (n <= 0) {
        cerr << "Error in DataCheck: n should be positive" << endl;
        exit(EXIT_SUCCESS); 
    }
    if (rectangle.width() != rectangle.height()) {
        cerr << "Error in DataCheck: the rectangle should be a square" << endl;
        exit(EXIT_SUCCESS); 
    }
    for (int i = 0; i < rectangleBoundaryCondition.size(); i++) {
        if (rectangleBoundaryCondition[i] != "Dirichlet" && rectangleBoundaryCondition[i] != "Neumann") {
            cerr << "Error in DataCheck: unknown rectangle boudary condtion" << endl;
            exit(EXIT_SUCCESS);
        }
    }
    if (circleBoundaryCondition != "Dirichlet" && circleBoundaryCondition != "Neumann") {
        cerr << "Error in DataCheck: unknown circle boudary condtion" << endl;
        exit(EXIT_SUCCESS);
    }
    cout << "The format of data is right" << endl;
}

/// @brief Check whether the calculation region is vaild
void ParameterCheck::MeshCheck(const int n, const Rectangle& rectangle, const Circle& circle) {
    cout << "Checking whether the mesh is valid..." << endl;
    // the circle should include at least 4 circle points
    int circle_points = 0;
    double h = max(rectangle.width(), rectangle.height()) / (n + 1);
    for (int i = 0; i < _Mesh.row(); i++) {
        for (int j = 0; j < _Mesh.col(); j++) {
            if (_Mesh(i, j) == BOUNDARY_POINT && 
                IS_IN_CIRCLE(i * h, j * h, circle.center_x(), circle.center_y(), circle.radius())) {
                circle_points++;
            }
        }
    }
    if (circle.radius() > 0 && circle_points < 4) {
        cerr << "Error in MeshCheck: the circle should include at least 4 circle points" << endl;
        exit(EXIT_SUCCESS); 
    }

    // the calculation region needs to be connected
    ConnectedRegion CalculationRegion(_Mesh);
    CalculationRegion.GenerateConnectedImage();
    _ConnectedMesh = CalculationRegion.Image();
    if (!CalculationRegion.IsConnected()) {
        cerr << "Error in MeshCheck: the calculation region is not connected" << endl;
        exit(EXIT_SUCCESS);
    }
}

/// @brief Check the data and the mesh generated for the shapes
void ParameterCheck::Check(const TopologyInfo& topologyInfo) {
    DataCheck(topologyInfo.n(), topologyInfo.rectangle(), topologyInfo.circle(),
              topologyInfo.rectangle_boundary_condition(), topologyInfo.circle_boundary_condition());
    GenerateMesh(topologyInfo.n(), topologyInfo.rectangle(), topologyInfo.circle());
    MeshCheck(topologyInfo.n(), topologyInfo.rectangle(), topologyInfo.circle());
    cout << "The data and corresponding mesh is valid for this program" << endl;
}