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

int main(int argc, char *argv[]) {
    clock_t start, end;
    JsonInfo JsonInfo("template.json");
    const IVPInfo& IVPInfo = JsonInfo.GetIVPInfo();
    JsonInfo.PrintJsonInfo(IVPInfo);
    auto& factory = TimeIntegratorFactory::GetInstance();
    TimeIntegrator* integrator = factory.CreateTimeIntegrator(IVPInfo.MethodName(), IVPInfo.p());
    if(integrator){
        cout << "=============================================================" << endl;
        start = clock();
        integrator->solve(IVPInfo);
        end = clock();
        delete integrator;
        cout << "Execution time: " << ((double)(end - start)) / CLOCKS_PER_SEC << " seconds" << endl;
        cout << "=============================================================" << endl;
    }
    return 0;
}