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
#include "TopologyInfo.hpp"

using namespace std;

/// @brief JsonInfo class
/// @details This class is used to read and check the json file
class JsonInfo {
private:
    Json::Value _root;
    int n() const {return _root["n"].asInt();}
    double rectangle_width() const {return _root["Rectangle"]["Len"][0].asDouble();}
    double rectangle_height() const {return _root["Rectangle"]["Len"][1].asDouble();}
    double circle_center_x() const {return _root["Circle"]["Center"][0].asDouble();}
    double circle_center_y() const {return _root["Circle"]["Center"][1].asDouble();}
    double circle_radius() const {return _root["Circle"]["Radius"].asDouble();}
    const vector<string> rectangle_boundary_condition() const {
        const Json::Value& boundaryCondition = _root["Rectangle"]["BoundaryCondition"];
        vector<string> rectangle_boundary_condition(4);
        rectangle_boundary_condition[0] = boundaryCondition["Left"].asString();
        rectangle_boundary_condition[1] = boundaryCondition["Right"].asString();
        rectangle_boundary_condition[2] = boundaryCondition["Up"].asString();
        rectangle_boundary_condition[3] = boundaryCondition["Down"].asString();
        return rectangle_boundary_condition;
    }
    const string circle_boundary_condition() const {return _root["Circle"]["BoundaryCondition"].asString();}
public:
    JsonInfo() = default;
    JsonInfo(const string &filename);
    void InitJsonInfo(const string &filename);
    TopologyInfo GetTopologyInfo() const;
    void PrintJsonInfo() const;
};

#endif