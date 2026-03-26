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

/// @brief Initialize from json file and cheak the data
/// @param filename
void JsonInfo::InitJsonInfo(const string &filename) {
    Json::Value _root = OpenJsonFile(filename);
    InitFromJsonFile(_root);
}

/// @brief Open the json file and parse the data
/// @param filename 
/// @return Json::Value root, a data structure defined in jsoncpp
Json::Value JsonInfo::OpenJsonFile(const string &filename) const {
    ifstream infile(filename);
    if(!infile.is_open()) {
        cerr << "Error in ReadJsonFile: fail to open file" << endl; 
        exit(EXIT_SUCCESS);
    }       

    Json::CharReaderBuilder ReaderBuilder;
    ReaderBuilder["emitUTF8"] = true;
    Json::Value root;

    string errs;
    if (!Json::parseFromStream(ReaderBuilder, infile, &root, &errs)) {
        cerr << "Error in ReadJsonFile: fail to parse Json" << endl;
        exit(EXIT_SUCCESS); 
    }

    cout << "The data has been read from " << filename << endl;
    infile.close();
    return root;
}

/// @brief Read the data from json file
/// @param root 
void JsonInfo::InitFromJsonFile(const Json::Value &root) {
    _n = root["n"].asInt();
    _RectangleBoundaryCondition = root["Rectangle"]["BoundaryCondition"].asString();
    _CircleBoundaryCondition = root["Circle"]["BoundaryCondition"].asString();
    _RectangleLen.first = root["Rectangle"]["Len"][0].asDouble(), _RectangleLen.second = root["Rectangle"]["Len"][1].asDouble();
    _CircleCenter.first = root["Circle"]["Center"][0].asDouble(), _CircleCenter.second = root["Circle"]["Center"][1].asDouble();
    _CircleRadius = root["Circle"]["Radius"].asDouble();
}

/// @brief Print the necessary information into terminal
void JsonInfo::PrintJsonInfo() const {
    cout << "========================================" << endl;
    cout << "n: " << _n << endl;
    cout << "rectangle length: width = " << _RectangleLen.first << ", height = " << _RectangleLen.second << endl;
    cout << "rectangle boundary condtion: " << _RectangleBoundaryCondition << endl;
    cout << "circle center: (" << _CircleCenter.first << ", " << _CircleCenter.second << ")" << endl;
    cout << "circle radius: " << _CircleRadius << endl;
    cout << "circle boundary condtion: " << _CircleBoundaryCondition << endl;
    cout << "========================================" << endl;
}
