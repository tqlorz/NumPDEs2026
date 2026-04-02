/**
 * @file          FileIO.cpp
 * @author        zsh945
 * @date          2026-03-25
 */

#include "FileIO.hpp"

/// @brief JsonInfo constructor
/// @param filename 
JsonInfo::JsonInfo(const string &filename) {
    InitJsonInfo(filename);
}

/// @brief Open the json file and parse the data
/// @param filename
void JsonInfo::InitJsonInfo(const string &filename) {
    ifstream infile(filename);
    if(!infile.is_open()) {
        cerr << "Error in ReadJsonFile: fail to open file" << endl; 
        exit(EXIT_SUCCESS);
    }       

    Json::CharReaderBuilder ReaderBuilder;
    ReaderBuilder["emitUTF8"] = true;

    string errs;
    if (!Json::parseFromStream(ReaderBuilder, infile, &_root, &errs)) {
        cerr << "Error in ReadJsonFile: fail to parse Json" << endl;
        exit(EXIT_SUCCESS); 
    }

    cout << "The data has been read from " << filename << endl;
    infile.close();
}

TopologyInfo JsonInfo::GetTopologyInfo() const {
    int _n = n();
    Rectangle _Rectangle(0, 0, rectangle_width(), rectangle_height());
    Circle _Circle(circle_center_x(), circle_center_y(), circle_radius());
    vector<string> _RectangleBoundaryCondition = rectangle_boundary_condition();
    string _CircleBoundaryCondition = circle_boundary_condition();
    return TopologyInfo(_n, _Rectangle, _Circle, _RectangleBoundaryCondition, _CircleBoundaryCondition);
}

/// @brief Print the necessary information into terminal
void JsonInfo::PrintJsonInfo(const int FunctionLabel) const {
    cout << "=============================================================" << endl;
    switch (FunctionLabel) {
        case 1: {cout << "function: u = exp(y+sin(x))" << endl; break;}
        case 2: {cout << "function: u = x(1-x)y(1-y)" << endl; break;}
        case 3: {cout << "function: u = sin(x)+sin(y)" << endl; break;}
        default: {
                cerr << "Error in main: the function label is invalid" << endl; 
                exit(EXIT_SUCCESS);
        }
    }
    cout << "n: " << n() << endl;
    cout << "rectangle length: width = " << rectangle_width() << ", height = " << rectangle_height() << endl;
    cout << "rectangle boundary condtion: " << endl;
    cout << "    Left: " << rectangle_boundary_condition()[0] << endl;
    cout << "    Right: " << rectangle_boundary_condition()[1] << endl;
    cout << "    Up: " << rectangle_boundary_condition()[2] << endl;
    cout << "    Down: " << rectangle_boundary_condition()[3] << endl;
    cout << "circle center: (" << circle_center_x() << ", " << circle_center_y() << ")" << endl;
    cout << "circle radius: " << circle_radius() << endl;
    cout << "circle boundary condtion: " << circle_boundary_condition() << endl;
    cout << "=============================================================" << endl;
}
