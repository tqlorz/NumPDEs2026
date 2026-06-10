/**
 * @file          TimeIntegratorFactory.hpp
 * @author        zsh945
 * @date          2026-04-24
 * @brief         TimeIntegratorFactory class declaration
 */

#ifndef _TIMEINTEGRATORFACTORY_
#define _TIMEINTEGRATORFACTORY_

#include <map>
#include <string>
#include <iostream>
#include "TimeIntegrator.hpp"
#include "LinearMultistep.hpp"
#include "TimeIntegrator.hpp"

using namespace std;

/// @brief The factory class for creating time integrators
class TimeIntegratorFactory{
public:
    using CreateTimeIntegratorCallBack = TimeIntegrator* (*)(int p);
private:
    using CallbackMap = map<string, CreateTimeIntegratorCallBack>;
    
private:
    CallbackMap _callbacks;
    TimeIntegratorFactory() = default;
    TimeIntegratorFactory(const TimeIntegratorFactory&) = default;
    TimeIntegratorFactory& operator = (const TimeIntegratorFactory&) = default;
    ~TimeIntegratorFactory() = default;    
public:
    void RegisterTimeIntegrator(const string &ID, CreateTimeIntegratorCallBack createFn){
        _callbacks[ID] = createFn;
    }
    TimeIntegrator* CreateTimeIntegrator(const string &ID, int p = 2){
        if(!_callbacks.count(ID)){
            cerr << "Error in TimeIntegratorFactory::CreateTimeIntegrator: No such TimeIntegrator called '" << ID << "'." << endl;
            return nullptr;
        }
        return _callbacks[ID](p);
    }
    static TimeIntegratorFactory& GetInstance(){
        static TimeIntegratorFactory singleton;
        return singleton;
    }
};

// Register the ABMs to the factory
static void Register_ABMs(void)__attribute__((constructor));
inline void Register_ABMs(){
    auto& factory = TimeIntegratorFactory::GetInstance();
    factory.RegisterTimeIntegrator("ABMs", [](int p){ 
        switch (p){
            case 1:
                return (TimeIntegrator*) new ABMs<1>();
                break;
            case 2:
                return (TimeIntegrator*) new ABMs<2>();
                break;
            case 3:
                return (TimeIntegrator*) new ABMs<3>();
                break;
            case 4:
                return (TimeIntegrator*) new ABMs<4>();
                break;
            default:
                return (TimeIntegrator*) nullptr;
        }
    });
}

// Register the AMMs to the factory
static void Register_AMMs(void)__attribute__((constructor));
inline void Register_AMMs(){
    auto& factory = TimeIntegratorFactory::GetInstance();
    factory.RegisterTimeIntegrator("AMMs", [](int p){
        switch (p){
            case 2:
                return (TimeIntegrator*) new AMMs<2>();
                break;
            case 3:
                return (TimeIntegrator*) new AMMs<3>();
                break;
            case 4:
                return (TimeIntegrator*) new AMMs<4>();
                break;
            case 5:
                return (TimeIntegrator*) new AMMs<5>();
                break;
            default:
                return (TimeIntegrator*) nullptr;
        }
    });
}

// Register the BDFs to the factory
static void Register_BDFs(void)__attribute__((constructor));
inline void Register_BDFs(){
    auto& factory = TimeIntegratorFactory::GetInstance();
    factory.RegisterTimeIntegrator("BDFs", [](int p){
        switch (p){
            case 1:
                return (TimeIntegrator*) new BDFs<1>();
                break;
            case 2:
                return (TimeIntegrator*) new BDFs<2>();
                break;
            case 3:
                return (TimeIntegrator*) new BDFs<3>();
                break;
            case 4:
                return (TimeIntegrator*) new BDFs<4>();
                break;
            default:
                return (TimeIntegrator*) nullptr;
        }
    });
}

// Register the 4-order classical RK to the factory
static void Register_classicalRK(void)__attribute__((constructor));
inline void Register_classicalRK(){
    auto& factory = TimeIntegratorFactory::GetInstance();
    factory.RegisterTimeIntegrator("classicalRK", [](int p){ 
        switch (p){
            case 1:
                return (TimeIntegrator*) new classicalRK<1>();
                break;
            case 2:
                return (TimeIntegrator*) new classicalRK<2>();
                break;
            case 3:
                return (TimeIntegrator*) new classicalRK<3>();
                break;
            case 4:
                return (TimeIntegrator*) new classicalRK<4>();
                break;
            case 5:
                return (TimeIntegrator*) new classicalRK<5>();
                break;
            default:
                return (TimeIntegrator*) nullptr;
        }
    });
}

// Register the ESDIRK to the factory
static void Register_ESDIRK(void)__attribute__((constructor));
inline void Register_ESDIRK(){
    auto& factory = TimeIntegratorFactory::GetInstance();
    factory.RegisterTimeIntegrator("ESDIRK", [](int p){ 
        switch (p){
            case 4:
                return (TimeIntegrator*) new ESDIRK();
                break;
            default:
                return (TimeIntegrator*) nullptr;
        }
    });
}

// Register the Gauss-Legendre Runge-Kutta methods to the factory
static void Register_GaussLegendreRK(void)__attribute__((constructor));
inline void Register_GaussLegendreRK(){
    auto& factory = TimeIntegratorFactory::GetInstance();
    factory.RegisterTimeIntegrator("GaussLegendreRK", [](int p){ 
        switch (p){
            case 4:
                return (TimeIntegrator*) new GaussLegendreRK<4>();
                break;
            case 6:
                return (TimeIntegrator*) new GaussLegendreRK<6>();
                break;
            case 8:
                return (TimeIntegrator*) new GaussLegendreRK<8>();
                break;
            case 10:
                return (TimeIntegrator*) new GaussLegendreRK<10>();
                break;
            default:
                return (TimeIntegrator*) nullptr;
        }
    });
}

// Register the Fehlberg45 to the factory
static void Register_Fehlberg45(void)__attribute__((constructor));
inline void Register_Fehlberg45(){
    auto& factory = TimeIntegratorFactory::GetInstance();
    factory.RegisterTimeIntegrator("Fehlberg45", [](int p){ 
        switch (p){
            case 4:
                return (TimeIntegrator*) new Fehlberg45();
                break;
            case 5:
                return (TimeIntegrator*) new Fehlberg45();
                break;
            default:
                return (TimeIntegrator*) nullptr;
        }
    });
}

// Register the DormandPrince54 to the factory
static void Register_DormandPrince54(void)__attribute__((constructor));
inline void Register_DormandPrince54(){
    auto& factory = TimeIntegratorFactory::GetInstance();
    factory.RegisterTimeIntegrator("DormandPrince54", [](int p){ 
        switch (p){
            case 4:
                return (TimeIntegrator*) new DormandPrince54();
                break;
            case 5:
                return (TimeIntegrator*) new DormandPrince54();
                break;
            default:
                return (TimeIntegrator*) nullptr;
        }
    });
}


#endif
