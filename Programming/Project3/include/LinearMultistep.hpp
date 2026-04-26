/**
 * @file          LinearMutistep.hpp
 * @author        zsh945
 * @date          2026-04-25
 */

#ifndef _LINEARMULTISTEP_
#define _LINEARMULTISTEP_

#include <algorithm>
#include "TimeIntegrator.hpp"
#include "MacroDef.hpp"

using namespace std;

// =========================================================================
/// @brief Adams-Bashforth Methods
/// @tparam p 
template <int p>
class ABMs : public TimeIntegrator {
private:
    vector<Array> Initialize(const IVPInfo& IVPInfo);
public:
    ABMs() = default;
    ~ABMs() = default;
    vector<Array> OneStep(const IVPInfo& IVPInfo, 
                   const vector<Array>& u_history, const double t_n);
    void solve(const IVPInfo& IVPInfo) override;
};

template <int p>
void ABMs<p>::solve(const IVPInfo& IVPInfo){
    cout << "Solving the IVP with ABMs of order " << p << " ..." << endl;
    double TimeStep = IVPInfo.TimeStep();
    double TotalTime = IVPInfo.TotalTime();
    int NumSteps = static_cast<int>(floor(TotalTime / TimeStep));
    string filename = "output/data/" + IVPInfo.MethodName() + "_p" + to_string(p) + "_k" + to_string(TimeStep) + ".txt";
    // Compute the initial history using a classical Runge-Kutta method of order 4
    int s = Coefficients_ABMs[p-1].p2s;
    vector<Array> u_history(s+1);
    u_history = Initialize(IVPInfo);
    double t_n = 0;
    for (int i = 0; i <= s-1; i++){
        PrintResultFile(filename, u_history[i], t_n);
        t_n += TimeStep;
    }
    // At this time, t_n = s * TimeStep
    // Compute the next steps using the ABMs
    for (int i = s; i <= NumSteps; i++){
        u_history = OneStep(IVPInfo, u_history, t_n - s * TimeStep);
        PrintResultFile(filename, u_history[s-1], t_n);
        t_n += TimeStep;
    }
    cout << "Finished solving this IVP!" << endl;
    cout << "The result has been printed to " << filename << endl;
}

template <int p>
vector<Array> ABMs<p>::OneStep(const IVPInfo& IVPInfo, 
                        const vector<Array>& u_history, const double t_n){
    double mu = IVPInfo.mu();
    double TimeStep = IVPInfo.TimeStep();
    const Function<Array, Array, double, double>& func = IVPInfo.func();
    // Compute the n+s-th step using the history of the previous s steps
    int s = Coefficients_ABMs[p-1].p2s;
    vector<Array> res(s+1);
    res = u_history;
    res[s] = u_history[s-1];
    for (int i = 0; i <= s-1; i++){
        res[s] += TimeStep * Coefficients_ABMs[p-1].beta[i] * func(res[i], t_n + i * TimeStep, mu);
    }
    // Rotate the history to prepare for the next step
    rotate(res.begin(), res.begin() + 1, res.end());
    return res;
}

template <int p>
vector<Array> ABMs<p>::Initialize(const IVPInfo& IVPInfo){
    classicalRK<4> integrator;
    int s = Coefficients_ABMs[p-1].p2s;
    vector<Array> u_history(s+1);
    
    double mu = IVPInfo.mu();
    double TimeStep = IVPInfo.TimeStep();
    u_history[0] = IVPInfo.InitialValue();   
    double t_n = 0;
    for (int i = 1; i <= s-1; i++){
        u_history[i] = integrator.OneStep(IVPInfo, u_history[i-1], t_n);
        t_n += TimeStep;
    }
    return u_history;
}

// =========================================================================
/// @brief Adams-Moulton Methods
/// @tparam p 
template <int p>
class AMMs : public TimeIntegrator {
private:
    vector<Array> Initialize(const IVPInfo& IVPInfo);
public:
    AMMs() = default;
    ~AMMs() = default;
    vector<Array> OneStep(const IVPInfo& IVPInfo, 
                   const vector<Array>& u_history, const double t_n);
    void solve(const IVPInfo& IVPInfo) override;
};

template <int p>
void AMMs<p>::solve(const IVPInfo& IVPInfo){
    cout << "Solving the IVP with AMMs of order " << p << " ..." << endl;
    double TimeStep = IVPInfo.TimeStep();
    double TotalTime = IVPInfo.TotalTime();
    int NumSteps = static_cast<int>(floor(TotalTime / TimeStep));
    string filename = "output/data/" + IVPInfo.MethodName() + "_p" + to_string(p) + "_k" + to_string(TimeStep) + ".txt";
    // Compute the initial history using a classical Runge-Kutta method of order 4
    int s = Coefficients_AMMs[p-1].p2s;
    vector<Array> u_history(s+1);
    u_history = Initialize(IVPInfo);
    double t_n = 0;
    for (int i = 0; i <= s-1; i++){
        PrintResultFile(filename, u_history[i], t_n);
        t_n += TimeStep;
    }
    // At this time, t_n = s * TimeStep
    // Compute the next steps using the AMMs
    for (int i = s; i <= NumSteps; i++){
        u_history = OneStep(IVPInfo, u_history, t_n - s * TimeStep);
        PrintResultFile(filename, u_history[s-1], t_n);
        t_n += TimeStep;
    }
    cout << "Finished solving this IVP!" << endl;
    cout << "The result has been printed to " << filename << endl;
}

template <int p>
vector<Array> AMMs<p>::OneStep(const IVPInfo& IVPInfo, 
                        const vector<Array>& u_history, const double t_n){
    double mu = IVPInfo.mu();
    double TimeStep = IVPInfo.TimeStep();
    const Function<Array, Array, double, double>& func = IVPInfo.func();
    // Compute the n+s-th step using ABMs to predict the n+s-th step, and then use AMMs to correct it
    int s = Coefficients_AMMs[p-1].p2s;
    vector<Array> res(s+1);
    classicalRK<4> predictor;
    res = u_history;
    res[s] = predictor.OneStep(IVPInfo, res[s-1], t_n);
    int iterations = 3; // Number of iterations for the corrector
    while (iterations > 0) {
        Array corrected = res[s-1];
        for (int i = 0; i <= s; i++){
            corrected += TimeStep * Coefficients_AMMs[p-1].beta[i] * func(res[i], t_n + i * TimeStep, mu);
        }
        res[s] = corrected;
        iterations--;
    }
    
    // Rotate the history to prepare for the next step
    rotate(res.begin(), res.begin() + 1, res.end());
    return res;
}

template <int p>
vector<Array> AMMs<p>::Initialize(const IVPInfo& IVPInfo){
    classicalRK<4> integrator;
    int s = Coefficients_AMMs[p-1].p2s;
    vector<Array> u_history(s+1);
    
    double mu = IVPInfo.mu();
    double TimeStep = IVPInfo.TimeStep();
    u_history[0] = IVPInfo.InitialValue();   
    double t_n = 0;
    for (int i = 1; i <= s-1; i++){
        u_history[i] = integrator.OneStep(IVPInfo, u_history[i-1], t_n);
        t_n += TimeStep;
    }
    return u_history;
}

// =========================================================================
/// @brief Backward Differentiation Formulas
/// @tparam p
template <int p>
class BDFs : public TimeIntegrator {
private:
    vector<Array> Initialize(const IVPInfo& IVPInfo);
public:
    BDFs() = default;
    ~BDFs() = default;
    vector<Array> OneStep(const IVPInfo& IVPInfo, 
                   const vector<Array>& u_history, const double t_n);
    void solve(const IVPInfo& IVPInfo) override;
};

template <int p>
void BDFs<p>::solve(const IVPInfo& IVPInfo){
    cout << "Solving the IVP with BDFs of order " << p << " ..." << endl;
    double TimeStep = IVPInfo.TimeStep();
    double TotalTime = IVPInfo.TotalTime();
    int NumSteps = static_cast<int>(floor(TotalTime / TimeStep));
    string filename = "output/data/" + IVPInfo.MethodName() + "_p" + to_string(p) + "_k" + to_string(TimeStep) + ".txt";
    // Compute the initial history using a classical Runge-Kutta method of order 4
    int s = Coefficients_BDFs[p-1].p2s;
    vector<Array> u_history(s+1);
    u_history = Initialize(IVPInfo);
    double t_n = 0;
    for (int i = 0; i <= s-1; i++){
        PrintResultFile(filename, u_history[i], t_n);
        t_n += TimeStep;
    }
    // At this time, t_n = s * TimeStep
    // Compute the next steps using the BDFs
    for (int i = s; i <= NumSteps; i++){
        u_history = OneStep(IVPInfo, u_history, t_n - s * TimeStep);
        PrintResultFile(filename, u_history[s-1], t_n);
        t_n += TimeStep;
    }
    cout << "Finished solving this IVP!" << endl;
    cout << "The result has been printed to " << filename << endl;
}

template <int p>
vector<Array> BDFs<p>::OneStep(const IVPInfo& IVPInfo, 
                        const vector<Array>& u_history, const double t_n){
    double mu = IVPInfo.mu();
    double TimeStep = IVPInfo.TimeStep();
    const Function<Array, Array, double, double>& func = IVPInfo.func();
    // Compute the n+s-th step using ABMs to predict the n+s-th step, and then use BDFs to correct it
    int s = Coefficients_BDFs[p-1].p2s;
    vector<Array> res(s+1);
    classicalRK<4> predictor;
    res = u_history;
    res[s] = predictor.OneStep(IVPInfo, res[s-1], t_n);
    int iterations = 3; // Number of iterations for the corrector
    while (iterations > 0) {
        Array corrected = TimeStep * Coefficients_BDFs[p-1].beta[s] * func(res[s], t_n + s * TimeStep, mu);
        for (int i = 0; i <= s-1; i++){
            corrected -= Coefficients_BDFs[p-1].alpha[i] * res[i];
        }
        res[s] = corrected;
        iterations--;
    }
    
    // Rotate the history to prepare for the next step
    rotate(res.begin(), res.begin() + 1, res.end());
    return res;
}

template <int p>
vector<Array> BDFs<p>::Initialize(const IVPInfo& IVPInfo){
    classicalRK<4> integrator;
    int s = Coefficients_BDFs[p-1].p2s;
    vector<Array> u_history(s+1);
    
    double mu = IVPInfo.mu();
    double TimeStep = IVPInfo.TimeStep();
    u_history[0] = IVPInfo.InitialValue();   
    double t_n = 0;
    for (int i = 1; i <= s-1; i++){
        u_history[i] = integrator.OneStep(IVPInfo, u_history[i-1], t_n);
        t_n += TimeStep;
    }
    return u_history;
}

#endif