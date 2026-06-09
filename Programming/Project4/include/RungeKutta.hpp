/**
 * @file          RungeKutta.hpp
 * @author        zsh945
 * @date          2026-04-25
 */
#ifndef _RUNGEKUTTA_
#define _RUNGEKUTTA_

#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>
#include "TimeIntegrator.hpp"
#include "Norm.hpp"
#include "Coefficients.hpp"

using namespace std;

// =========================================================================
/// @brief The base class for Runge-Kutta time integrators
class RungeKutta : public TimeIntegrator {
protected:
    Array u_end;
public:    
    RungeKutta() = default;
    ~RungeKutta() = default;
    virtual void solve(const IVPInfo& IVPInfo) = 0;
    Array solution() const { return u_end; }
};

// =========================================================================
/// @brief The classical Runge-Kutta methods
/// @tparam p
/// @note usually, the classical Runge-Kutta is referred to the 4th-order one
template <int p>
class classicalRK : public RungeKutta {
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
    u_end = u_n;
    cout << "Finished solving this IVP!" << endl;
    cout << "The result has been printed to " << filename << endl;
}

template <int p>
Array classicalRK<p>::OneStep(const IVPInfo& IVPInfo, 
                                const Array& u_n, const double t_n){
    double mu = IVPInfo.mu();
    double TimeStep = IVPInfo.TimeStep();
    const Function<Array, Array, double, double>& func = IVPInfo.func();
    int index = p2index(Coefficients_classicalRK, p);
    int s = Coefficients_classicalRK[index].s;
    // Compute the intermediate stages
    vector<Array> y(s);
    for (int i = 0; i < s; i++){
        Array u = u_n;
        double t = t_n + Coefficients_classicalRK[index].c[i] * TimeStep;
        for (int j = 0; j < i; j++){
            u += TimeStep * Coefficients_classicalRK[index].a[i][j] * y[j];
        }
        y[i] = func(u, t, mu);
    }
    // Compute the next step
    Array res = u_n;
    for (int i = 0; i < s; i++){
        res += TimeStep * Coefficients_classicalRK[index].b[i] * y[i];
    }
    return res;
}

// =========================================================================
/// @brief The ESDIRK methods
/// @note a six-stage fourth-order ESDIRK method in Example 13.52
class ESDIRK : public RungeKutta {
private:
public:
    ESDIRK() = default;
    ~ESDIRK() = default;
    Array OneStep(const IVPInfo& IVPInfo, 
                const Array& u_n, const double t_n);
    void solve(const IVPInfo& IVPInfo) override;
};

inline void ESDIRK::solve(const IVPInfo& IVPInfo){
    
}

inline Array ESDIRK::OneStep(const IVPInfo& IVPInfo, 
                                const Array& u_n, const double t_n){
    return u_n;
}

// =========================================================================
/// @brief The Gauss-Legendre Runge-Kutta methods
/// @tparam p
template <int p>
class GaussLegendreRK : public RungeKutta {
private:
public:
    GaussLegendreRK() = default;
    ~GaussLegendreRK() = default;
    Array OneStep(const IVPInfo& IVPInfo, 
                const Array& u_n, const double t_n);
    void solve(const IVPInfo& IVPInfo) override;
};

template <int p>
void GaussLegendreRK<p>::solve(const IVPInfo& IVPInfo){

}

template <int p>
Array GaussLegendreRK<p>::OneStep(const IVPInfo& IVPInfo, 
                                const Array& u_n, const double t_n){    
    return u_n;
}

// =========================================================================
/// @brief The Fehlberg's method
class Fehlberg45 : public RungeKutta {
private:
public:    
    Fehlberg45() = default;
    ~Fehlberg45() = default;
    pair<double, Array> EmbeddedOneStep(const IVPInfo& IVPInfo, 
                const Array& u_n, const double t_n, const double TimeStep);
    Array OneStep(const IVPInfo& IVPInfo, 
                const Array& u_n, const double t_n, const double TimeStep, const int p);
    void solve(const IVPInfo& IVPInfo) override;
};

inline void Fehlberg45::solve(const IVPInfo& IVPInfo){
    cout << "Solving the IVP with Fehlberg45 ..." << endl;
    double TimeStep = IVPInfo.TimeStep();
    double TotalTime = IVPInfo.TotalTime();
    string filename = "output/data/" + IVPInfo.MethodName() + ".txt";
    // Compute the solution at each time step
    Array u_n = IVPInfo.InitialValue();
    double t_n = 0;
    while (t_n < TotalTime){
        PrintResultFile(filename, u_n, t_n);
        TimeStep = EmbeddedOneStep(IVPInfo, u_n, t_n, TimeStep).first;
        u_n = EmbeddedOneStep(IVPInfo, u_n, t_n, TimeStep).second;
        t_n += TimeStep;
    }
    u_end = u_n;
    cout << "Finished solving this IVP!" << endl;
    cout << "The result has been printed to " << filename << endl;
}

inline pair<double, Array> Fehlberg45::EmbeddedOneStep(const IVPInfo& IVPInfo, 
                                const Array& u_n, const double t_n, const double TimeStep){
    // Set the tolerance for adaptive time stepping
    int q = min(4, 5);
    double rho_max = 1.5;
    double rho_min = 0.2;
    double rho = 0.8;
    double tol_abs = 1e-10;
    double tol_rel = 1e-5;
    Array E_abs = Array(u_n.size(), tol_abs);
    Array E_rel = Array(u_n.size(), tol_rel);
    Array varepsilon = E_abs + E_rel * abs(u_n);
    // Compute the 4th-order and 5th-order solutions
    Array res = u_n;
    double TimeStep_new = TimeStep;
    LinfNorm linfNorm;
    double E_ind = 2.0;
    while (E_ind > 1.0){
        Array u_4 = OneStep(IVPInfo, u_n, t_n, TimeStep_new, 4);
        Array u_5 = OneStep(IVPInfo, u_n, t_n, TimeStep_new, 5);
        res = u_5;
        E_ind = linfNorm((u_5 - u_4) / varepsilon);
        TimeStep_new = TimeStep_new * min(rho_max,
                                            max(rho_min, rho * pow(1.0 / E_ind, 1.0 / (q + 1))));
    }  
    return make_pair(TimeStep_new, res);
}

inline Array Fehlberg45::OneStep(const IVPInfo& IVPInfo, 
                                const Array& u_n, const double t_n, 
                                const double TimeStep, const int p){
    double mu = IVPInfo.mu();
    const Function<Array, Array, double, double>& func = IVPInfo.func();
    int index = p2index(Coefficients_Fehlberg45, p);
    int s = Coefficients_Fehlberg45[index].s;
    // Compute the intermediate stages
    vector<Array> y(s);
    for (int i = 0; i < s; i++){
        Array u = u_n;
        double t = t_n + Coefficients_Fehlberg45[index].c[i] * TimeStep;
        for (int j = 0; j < i; j++){
            u += TimeStep * Coefficients_Fehlberg45[index].a[i][j] * y[j];
        }
        y[i] = func(u, t, mu);
    }
    // Compute the next step
    Array res = u_n;
    for (int i = 0; i < s; i++){
        res += TimeStep * Coefficients_Fehlberg45[index].b[i] * y[i];
    }
    return res;
}

// =========================================================================
/// @brief The Dormand-Prince method
class DormandPrince54 : public RungeKutta {
private:
public:    
    DormandPrince54() = default;
    ~DormandPrince54() = default;   
    pair<double, Array> EmbeddedOneStep(const IVPInfo& IVPInfo, 
                const Array& u_n, const double t_n, const double TimeStep);
    Array OneStep(const IVPInfo& IVPInfo, 
                const Array& u_n, const double t_n, const double TimeStep, const int p);
    void solve(const IVPInfo& IVPInfo) override;
};

inline void DormandPrince54::solve(const IVPInfo& IVPInfo){
    cout << "Solving the IVP with Dormand-Prince54 ..." << endl;
    double TimeStep = IVPInfo.TimeStep();
    double TotalTime = IVPInfo.TotalTime();
    string filename = "output/data/" + IVPInfo.MethodName() + ".txt";
    // Compute the solution at each time step
    Array u_n = IVPInfo.InitialValue();
    double t_n = 0;
    while (t_n < TotalTime){
        PrintResultFile(filename, u_n, t_n);
        TimeStep = EmbeddedOneStep(IVPInfo, u_n, t_n, TimeStep).first;
        u_n = EmbeddedOneStep(IVPInfo, u_n, t_n, TimeStep).second;
        t_n += TimeStep;
    }
    u_end = u_n;
    cout << "Finished solving this IVP!" << endl;
    cout << "The result has been printed to " << filename << endl;
}

inline pair<double, Array> DormandPrince54::EmbeddedOneStep(const IVPInfo& IVPInfo, 
                const Array& u_n, const double t_n, const double TimeStep){
    // Set the tolerance for adaptive time stepping
    int q = min(4, 5);
    double rho_max = 1.5;
    double rho_min = 0.2;
    double rho = 0.8;
    double tol_abs = 1e-10;
    double tol_rel = 1e-5;
    Array E_abs = Array(u_n.size(), tol_abs);
    Array E_rel = Array(u_n.size(), tol_rel);
    Array varepsilon = E_abs + E_rel * abs(u_n);
    // Compute the 4th-order and 5th-order solutions
    Array res = u_n;
    double TimeStep_new = TimeStep;
    LinfNorm linfNorm;
    double E_ind = 2.0;
    while (E_ind > 1.0){
        Array u_4 = OneStep(IVPInfo, u_n, t_n, TimeStep_new, 4);
        Array u_5 = OneStep(IVPInfo, u_n, t_n, TimeStep_new, 5);
        res = u_5;
        E_ind = linfNorm((u_5 - u_4) / varepsilon);
        TimeStep_new = TimeStep_new * min(rho_max,
                                            max(rho_min, rho * pow(1.0 / E_ind, 1.0 / (q + 1))));
    }  
    return make_pair(TimeStep_new, res);
}

inline Array DormandPrince54::OneStep(const IVPInfo& IVPInfo, 
                const Array& u_n, const double t_n, const double TimeStep, const int p){
    double mu = IVPInfo.mu();
    const Function<Array, Array, double, double>& func = IVPInfo.func();
    int index = p2index(Coefficients_DormandPrince54, p);
    int s = Coefficients_DormandPrince54[index].s;
    // Compute the intermediate stages
    vector<Array> y(s);
    for (int i = 0; i < s; i++){
        Array u = u_n;
        double t = t_n + Coefficients_DormandPrince54[index].c[i] * TimeStep;
        for (int j = 0; j < i; j++){
            u += TimeStep * Coefficients_DormandPrince54[index].a[i][j] * y[j];
        }
        y[i] = func(u, t, mu);
    }
    // Compute the next step
    Array res = u_n;
    for (int i = 0; i < s; i++){
        res += TimeStep * Coefficients_DormandPrince54[index].b[i] * y[i];
    }
    return res;
}

#endif