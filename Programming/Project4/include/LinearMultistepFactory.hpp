/**
 * @file          LinearMultistepFactory.hpp
 * @author        zsh945
 * @date          2026-06-01
 * @brief         LinearMultistepFactory class declaration
 */

#ifndef _LINEARMULTISTEPFACTORY_
#define _LINEARMULTISTEPFACTORY_

#include <map>
#include <string>
#include <iostream>
#include "TimeIntegrator.hpp"
#include "LinearMultistep.hpp"

using namespace std;

/// @brief The factory class for creating LinearMultistep time integrators
class LinearMultistepFactory{
public:
    using CreateLinearMultistepCallBack = LinearMultistep* (*)(int p);
private:
    using CallbackMap = map<string, CreateLinearMultistepCallBack>;
    
private:
    CallbackMap _callbacks;
    LinearMultistepFactory() = default;
    LinearMultistepFactory(const LinearMultistepFactory&) = default;
    LinearMultistepFactory& operator = (const LinearMultistepFactory&) = default;
    ~LinearMultistepFactory() = default;    
public:
    void RegisterLinearMultistep(const string &ID, CreateLinearMultistepCallBack createFn){
        _callbacks[ID] = createFn;
    }
    LinearMultistep* CreateLinearMultistep(const string &ID, int p = 2){
        if(!_callbacks.count(ID)){
            cerr << "Error in LinearMultistepFactory::CreateLinearMultistep: No such LinearMultistep called '" << ID << "'." << endl;
            return nullptr;
        }
        return _callbacks[ID](p);
    }
    static LinearMultistepFactory& GetInstance(){
        static LinearMultistepFactory singleton;
        return singleton;
    }
};

// Register the ABMs to the factory
static void Register_ABMs(void)__attribute__((constructor));
inline void Register_ABMs(){
    auto& factory = LinearMultistepFactory::GetInstance();
    factory.RegisterLinearMultistep("ABMs", [](int p){ 
        switch (p){
            case 1:
                return (LinearMultistep*) new ABMs<1>();
                break;
            case 2:
                return (LinearMultistep*) new ABMs<2>();
                break;
            case 3:
                return (LinearMultistep*) new ABMs<3>();
                break;
            case 4:
                return (LinearMultistep*) new ABMs<4>();
                break;
            default:
                return (LinearMultistep*) nullptr;
        }
    });
}

// Register the AMMs to the factory
static void Register_AMMs(void)__attribute__((constructor));
inline void Register_AMMs(){
    auto& factory = LinearMultistepFactory::GetInstance();
    factory.RegisterLinearMultistep("AMMs", [](int p){ 
        switch (p){
            case 2:
                return (LinearMultistep*) new AMMs<2>();
                break;
            case 3:
                return (LinearMultistep*) new AMMs<3>();
                break;
            case 4:
                return (LinearMultistep*) new AMMs<4>();
                break;
            case 5:
                return (LinearMultistep*) new AMMs<5>();
                break;
            default:
                return (LinearMultistep*) nullptr;
        }
    });
}

// Register the BDFs to the factory
static void Register_BDFs(void)__attribute__((constructor));
inline void Register_BDFs(){
    auto& factory = LinearMultistepFactory::GetInstance();
    factory.RegisterLinearMultistep("BDFs", [](int p){ 
        switch (p){
            case 1:
                return (LinearMultistep*) new BDFs<1>();
                break;
            case 2:
                return (LinearMultistep*) new BDFs<2>();
                break;
            case 3:
                return (LinearMultistep*) new BDFs<3>();
                break;
            case 4:
                return (LinearMultistep*) new BDFs<4>();
                break;
            default:
                return (LinearMultistep*) nullptr;
        }
    });
}

#endif
