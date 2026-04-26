/**
 * @file          RungeKutta.hpp
 * @author        zsh945
 * @date          2026-04-25
 */
#ifndef _RUNGEKUTTA_
#define _RUNGEKUTTA_

#include <cmath>
#include <iostream>
#include "TimeIntegrator.hpp"
#include "MacroDef.hpp"

using namespace std;

template <int p>
class classicalRK : public TimeIntegrator {
private:
public:
    classicalRK() = default;
    ~classicalRK() = default;
    Array OneStep(const IVPInfo& IVPInfo, 
                const Array& u_n, const double t_n);
    void solve(const IVPInfo& IVPInfo) override;
};

template <int p>
void classicalRK<p>::solve(const IVPInfo& IVPInfo){
    cout << "Solving the IVP with classicalRK of order " << p << " ..." << endl;
    double TimeStep = IVPInfo.TimeStep();
    double TotalTime = IVPInfo.TotalTime();
    int NumSteps = static_cast<int>(floor(TotalTime / TimeStep));
    string filename = "output/data/" + IVPInfo.MethodName() + "_p" + to_string(p) + "_k" + to_string(TimeStep) + ".txt";
    // Compute the solution at each time step
    Array u_n = IVPInfo.InitialValue();
    double t_n = 0;
    for (int i = 0; i <= NumSteps; i++){
        PrintResultFile(filename, u_n, t_n);
        t_n += TimeStep;
        u_n = OneStep(IVPInfo, u_n, t_n);
    }
    cout << "Finished solving this IVP!" << endl;
    cout << "The result has been printed to " << filename << endl;
}

template <int p>
Array classicalRK<p>::OneStep(const IVPInfo& IVPInfo, 
                                const Array& u_n, const double t_n){
    double mu = IVPInfo.mu();
    double TimeStep = IVPInfo.TimeStep();
    const Function<Array, Array, double, double>& func = IVPInfo.func();
    int s = Coefficients_classicalRK[p-1].p2s;
    // Compute the intermediate stages
    vector<Array> y(s);
    for (int i = 0; i < s; i++){
        Array u = u_n;
        double t = t_n + Coefficients_classicalRK[p-1].c[i] * TimeStep;
        for (int j = 0; j < i; j++){
            u += TimeStep * Coefficients_classicalRK[p-1].a[i][j] * y[j];
        }
        y[i] = func(u, t, mu);
    }
    // Compute the next step
    Array res = u_n;
    for (int i = 0; i < s; i++){
        res += TimeStep * Coefficients_classicalRK[p-1].b[i] * y[i];
    }
    return res;
}

#endif