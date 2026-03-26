/**
 * @file          FileIO.hpp
 * @author        zsh945
 * @date          2026-03-25
 * @brief         This file contains the declaration of the JsonInfo class, which is used to read and check the json file.
 */

#ifndef _FILEIO_
#define _FILEIO_

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <json/json.h>
#include "MacroDef.hpp"

using namespace std;

/// @brief JsonInfo class
/// @details This class is used to read and check the json file
class JsonInfo {
private:
    int _n;
    string _RectangleBoundaryCondition, _CircleBoundaryCondition;
    pair<double, double> _RectangleLen;
    pair<double, double> _CircleCenter;
    double _CircleRadius;
    Json::Value OpenJsonFile(const string &filename) const;
    void InitFromJsonFile(const Json::Value &root);
public:
    int n() const {return _n;}
    string rectangle_boundary_condition() const {return _RectangleBoundaryCondition;}
    string circle_boundary_condition() const {return _CircleBoundaryCondition;}
    double rectangle_width() const {return _RectangleLen.first;}
    double rectangle_height() const {return _RectangleLen.second;}
    double circle_center_x() const {return _CircleCenter.first;}
    double circle_center_y() const {return _CircleCenter.second;}
    double circle_radius() const {return _CircleRadius;}
    JsonInfo() = default;
    JsonInfo(const string &filename);
    void InitJsonInfo(const string &filename);
    void PrintJsonInfo() const;
};

#endif