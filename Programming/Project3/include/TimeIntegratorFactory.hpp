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
#include "RungeKutta.hpp"
#include "LinearMultistep.hpp"

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

// Register the LMM time integrators to the factory
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

// Register the RK time integrators to the factory
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
            default:
                return (TimeIntegrator*) nullptr;
        }
    });
}


#endif