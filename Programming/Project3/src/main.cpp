/**
 * @file          main.cpp
 * @author        zsh945
 * @date          2026-04-24
 */

#include <cmath>
#include "FileIO.hpp"

int main(int argc, char *argv[]) {
    JsonInfo JsonInfo("template.json");
    const IVPInfo& IVPInfo = JsonInfo.GetIVPInfo();
    JsonInfo.PrintJsonInfo(IVPInfo);
    return 0;
}