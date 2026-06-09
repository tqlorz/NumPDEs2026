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
#include "LinearMultistepFactory.hpp"
#include "RungeKuttaFactory.hpp"

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

#endif
