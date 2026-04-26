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
public:
    JsonInfo() = default;
    JsonInfo(const string &filename);
    void InitJsonInfo(const string &filename);
    void CheckJsonInfo(const int p, const int MethodLabel) const;
    const IVPInfo GetIVPInfo() const;
    void PrintJsonInfo(const IVPInfo& IVPInfo) const;
};

#endif