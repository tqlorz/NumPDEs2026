/**
 * @file          ResultAnalysis.cpp
 * @author        zsh945
 * @date          2026-05-1
 */

#include "ResultAnalysis.hpp"

void ResultAnalysis(const IVPInfo& ivpInfo, const int CaseLabel) {
    clock_t start, end;
    L1Norm l1Norm;
    L2Norm l2Norm;
    LinfNorm linfNorm;
    auto& factory = TimeIntegratorFactory::GetInstance();
    TimeIntegrator* integrator = factory.CreateTimeIntegrator(ivpInfo.MethodName(), ivpInfo.p());
    switch (CaseLabel) {
        case 1: {
            if(integrator){
                cout << "ivp:" << endl;
                cout << "=============================================================" << endl;
                start = clock();
                integrator->solve(ivpInfo);
                Array u_end = integrator->solution();
                end = clock();
                cout << "Execution time: " << ((double)(end - start)) / CLOCKS_PER_SEC << " seconds" << endl;
                cout << "=============================================================" << endl;
                cout << "Error analysis:" << endl;
                cout << "=============================================================" << endl;
                cout << "1-norm error: " << l1Norm(u_end - ivpInfo.InitialValue()) << endl;
                cout << "2-norm error: " << l2Norm(u_end - ivpInfo.InitialValue()) << endl;
                cout << "Infinity-norm error: " << linfNorm(u_end - ivpInfo.InitialValue()) << endl;
                cout << "=============================================================" << endl;
                delete integrator;
            }
            break;
        }
        case 2: {
            if(integrator){
                cout << "ivp:" << endl;
                cout << "=============================================================" << endl;
                start = clock();
                integrator->solve(ivpInfo);
                end = clock();
                Array u_end = integrator->solution();
                cout << "Execution time: " << ((double)(end - start)) / CLOCKS_PER_SEC << " seconds" << endl;
                cout << "=============================================================" << endl;
                IVPInfo ivpInfo_refined(ivpInfo.MethodName(), ivpInfo.p(), ivpInfo.mu(), 
                                        ivpInfo.TimeStep()/2, ivpInfo.TotalTime(), ivpInfo.InitialValue(), ivpInfo.func());
                cout << "ivp on refined grid:" << endl;
                cout << "=============================================================" << endl;
                start = clock();
                integrator->solve(ivpInfo_refined);
                end = clock();
                Array u_end_refined = integrator->solution();
                cout << "Execution time: " << ((double)(end - start)) / CLOCKS_PER_SEC << " seconds" << endl;
                cout << "=============================================================" << endl;
                cout << "Error analysis:" << endl;
                cout << "=============================================================" << endl;
                cout << "1-norm error: " << l1Norm(u_end - u_end_refined) << endl;
                cout << "2-norm error: " << l2Norm(u_end - u_end_refined) << endl;
                cout << "Infinity-norm error: " << linfNorm(u_end - u_end_refined) << endl;
                cout << "=============================================================" << endl;
                delete integrator;
            }
            break;
        }
        default:{
            cerr << "Error in ResultAnalysis: the case label is invalid" << endl;
            exit(EXIT_SUCCESS);
            break;
        }
    }
}
