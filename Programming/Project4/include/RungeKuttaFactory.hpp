/**
 * @file          RungeKuttaFactory.hpp
 * @author        zsh945
 * @date          2026-06-01
 * @brief         RungeKuttaFactory class declaration
 */

#ifndef _RUNGEKUTTAFACTORY_
#define _RUNGEKUTTAFACTORY_

#include <map>
#include <string>
#include <iostream>
#include "TimeIntegrator.hpp"
#include "RungeKutta.hpp"

using namespace std;

/// @brief The factory class for creating Runge-Kutta time integrators
class RungeKuttaFactory{
public:
    using CreateRungeKuttaCallBack = RungeKutta* (*)(int p);
private:
    using CallbackMap = map<string, CreateRungeKuttaCallBack>;
    
private:
    CallbackMap _callbacks;
    RungeKuttaFactory() = default;
    RungeKuttaFactory(const RungeKuttaFactory&) = default;
    RungeKuttaFactory& operator = (const RungeKuttaFactory&) = default;
    ~RungeKuttaFactory() = default;    
public:
    void RegisterRungeKutta(const string &ID, CreateRungeKuttaCallBack createFn){
        _callbacks[ID] = createFn;
    }
    RungeKutta* CreateRungeKutta(const string &ID, int p = 2){
        if(!_callbacks.count(ID)){
            cerr << "Error in RungeKuttaFactory::CreateRungeKutta: No such RungeKutta called '" << ID << "'." << endl;
            return nullptr;
        }
        return _callbacks[ID](p);
    }
    static RungeKuttaFactory& GetInstance(){
        static RungeKuttaFactory singleton;
        return singleton;
    }
};

// Register the 4-order classical RK to the factory
static void Register_classicalRK(void)__attribute__((constructor));
inline void Register_classicalRK(){
    auto& factory = RungeKuttaFactory::GetInstance();
    factory.RegisterRungeKutta("classicalRK", [](int p){ 
        switch (p){
            case 4:
                return (RungeKutta*) new classicalRK<4>();
                break;
            default:
                return (RungeKutta*) nullptr;
        }
    });
}

// Register the ESDIRK to the factory
static void Register_ESDIRK(void)__attribute__((constructor));
inline void Register_ESDIRK(){
    auto& factory = RungeKuttaFactory::GetInstance();
    factory.RegisterRungeKutta("ESDIRK", [](int p){ 
        switch (p){
            case 4:
                return (RungeKutta*) new ESDIRK();
                break;
            default:
                return (RungeKutta*) nullptr;
        }
    });
}

// Register the Gauss-Legendre Runge-Kutta methods to the factory
static void Register_GaussLegendreRK(void)__attribute__((constructor));
inline void Register_GaussLegendreRK(){
    auto& factory = RungeKuttaFactory::GetInstance();
    factory.RegisterRungeKutta("GaussLegendreRK", [](int p){ 
        switch (p){
            case 4:
                return (RungeKutta*) new GaussLegendreRK<4>();
                break;
            case 6:
                return (RungeKutta*) new GaussLegendreRK<6>();
                break;
            case 8:
                return (RungeKutta*) new GaussLegendreRK<8>();
                break;
            case 10:
                return (RungeKutta*) new GaussLegendreRK<10>();
                break;
            default:
                return (RungeKutta*) nullptr;
        }
    });
}

// Register the Fehlberg45 to the factory
static void Register_Fehlberg45(void)__attribute__((constructor));
inline void Register_Fehlberg45(){
    auto& factory = RungeKuttaFactory::GetInstance();
    factory.RegisterRungeKutta("Fehlberg45", [](int p){ 
        switch (p){
            case 4:
                return (RungeKutta*) new Fehlberg45();
                break;
            case 5:
                return (RungeKutta*) new Fehlberg45();
                break;
            default:
                return (RungeKutta*) nullptr;
        }
    });
}

// Register the DormandPrince54 to the factory
static void Register_DormandPrince54(void)__attribute__((constructor));
inline void Register_DormandPrince54(){
    auto& factory = RungeKuttaFactory::GetInstance();
    factory.RegisterRungeKutta("DormandPrince54", [](int p){ 
        switch (p){
            case 4:
                return (RungeKutta*) new DormandPrince54();
                break;
            case 5:
                return (RungeKutta*) new DormandPrince54();
                break;
            default:
                return (RungeKutta*) nullptr;
        }
    });
}

#endif
