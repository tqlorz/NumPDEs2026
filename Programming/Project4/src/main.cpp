/**
 * @file          main.cpp
 * @author        zsh945
 * @date          2026-04-24
 */

#include <iostream>
#include <time.h>
#include <vector>
#include "Function.hpp"
#include "FileIO.hpp"
#include "TimeIntegratorFactory.hpp"
#include "RungeKutta.hpp"
#include "LinearMultistep.hpp"
#include "ResultAnalysis.hpp"
#include "NewtonMethod.hpp"
#include "FactorizationLU.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    JsonInfo JsonInfo("template.json");
    const IVPInfo& IVPInfo = JsonInfo.GetIVPInfo();
    JsonInfo.PrintJsonInfo(IVPInfo);
    ResultAnalysis(IVPInfo, JsonInfo.GetCaseLabel());
    return 0;
}