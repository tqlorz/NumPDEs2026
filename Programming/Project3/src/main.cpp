/**
 * @file          main.cpp
 * @author        zsh945
 * @date          2026-04-24
 */

#include <iostream>
#include <time.h>
#include "Function.hpp"
#include "FileIO.hpp"
#include "TimeIntegratorFactory.hpp"
#include "RungeKutta.hpp"
#include "LinearMultistep.hpp"
#include "ResultAnalysis.hpp"

int main(int argc, char *argv[]) {
    clock_t start, end;
    JsonInfo JsonInfo("template.json");
    const IVPInfo& IVPInfo = JsonInfo.GetIVPInfo();
    JsonInfo.PrintJsonInfo(IVPInfo);
    ResultAnalysis(IVPInfo, JsonInfo.GetCaseLabel());
    return 0;
}