/**
 * @file          FileIO.hpp
 * @author        zsh945
 * @date          2026-04-24
 * @brief         This file defines the JsonInfo class, which is used to read and check the json file
 */

#ifndef _FILEIO_
#define _FILEIO_

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <json/json.h>
#include <iomanip>
#include "IVPInfo.hpp"
#include "Array.hpp"
#include "Function.hpp"

using namespace std;

/// @brief JsonInfo class
/// @details This class is used to read and check the json file
class JsonInfo {
private:
    Json::Value _root;
    void CheckJsonInfo(const int p, const int MethodLabel) const;
public:
    JsonInfo() = default;
    JsonInfo(const string &filename);
    void InitJsonInfo(const string &filename);
    const IVPInfo GetIVPInfo() const;
    int GetCaseLabel() { return _root["Case_Label"].asInt(); }
    void PrintJsonInfo(const IVPInfo& ivpInfo) const;
};

#endif