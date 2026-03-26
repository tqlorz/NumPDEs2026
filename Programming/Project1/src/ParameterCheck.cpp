/**
 * @file          ParameterCheck.cpp
 * @author        zsh945
 * @date          2026-03-26
 */

#include "ParameterCheck.hpp"

/// @brief Generate the mesh for the ParameterCheck class. 
void ParameterCheck::GenerateMesh() {
    cout << "Generating mesh for check..." << endl;
    _h = max(_Rectangle.width(), _Rectangle.height()) / (_n + 1);
    _Mesh.init(_n + 2, _n + 2);
    for (int i = 0; i < _Mesh.row(); i++) {
        for (int j = 0; j < _Mesh.col(); j++) {
            if (SQUARE(i * _h - _Circle.center_x()) 
                + SQUARE(j * _h - _Circle.center_y()) <= SQUARE(_Circle.radius())) {
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
void ParameterCheck::DataCheck() const {
    if (_n <= 0) {
        cerr << "Error in DataCheck: n should be positive" << endl;
        exit(EXIT_SUCCESS); 
    }
    if (_RectangleBoundaryCondition != BOUNDARY_CONDITION[0] && _RectangleBoundaryCondition != BOUNDARY_CONDITION[1]) {
        cerr << "Error in DataCheck: unknown rectangle boudary condtion" << endl;
        exit(EXIT_SUCCESS);
    }
    if (_CircleBoundaryCondition != BOUNDARY_CONDITION[0] && _CircleBoundaryCondition != BOUNDARY_CONDITION[1]) {
        cerr << "Error in DataCheck: unknown circle boudary condtion" << endl;
        exit(EXIT_SUCCESS);
    }
    cout << "The format of data is right" << endl;
}

/// @brief Check whether the calculation region is vaild
void ParameterCheck::MeshCheck() const {
    cout << "Checking whether the mesh is valid..." << endl;
    // the circle should include at least 4 circle points
    int circle_points = 0;
    for (int i = 0; i < _Mesh.row(); i++) {
        for (int j = 0; j < _Mesh.col(); j++) {
            if (_Mesh(i, j) == BOUNDARY_POINT) circle_points++;
        }
    }
    if (circle_points < 4) {
        cerr << "Error in MeshCheck: the circle should include at least 4 circle points" << endl;
        exit(EXIT_SUCCESS); 
    }

    // the calculation region needs to be connected
    ConnectedRegion CalculationRegion(_Mesh);
    CalculationRegion.GenerateConnectedImage();
    if (!CalculationRegion.IsConnected()) {
        cerr << "Error in MeshCheck: the calculation region is not connected" << endl;
        exit(EXIT_SUCCESS);
    }
}

/// @brief Check the data and the mesh generated for the shapes
void ParameterCheck::Check() {
    DataCheck();
    GenerateMesh();
    MeshCheck();
    cout << "The data and corresponding mesh is valid for this program" << endl;
}