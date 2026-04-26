/**
 * @file          FileIO.cpp
 * @author        zsh945
 * @date          2026-04-24
 */

#include "FileIO.hpp"

/// @brief JsonInfo constructor
/// @param filename 
JsonInfo::JsonInfo(const string &filename) {
    InitJsonInfo(filename);
}

/// @brief Open the json file and parse the data
/// @param filename
void JsonInfo::InitJsonInfo(const string &filename) {
    ifstream infile(filename);
    if(!infile.is_open()) {
        cerr << "Error in ReadJsonFile: fail to open file" << endl; 
        exit(EXIT_SUCCESS);
    }       

    Json::CharReaderBuilder ReaderBuilder;
    ReaderBuilder["emitUTF8"] = true;

    string errs;
    if (!Json::parseFromStream(ReaderBuilder, infile, &_root, &errs)) {
        cerr << "Error in ReadJsonFile: fail to parse Json" << endl;
        exit(EXIT_SUCCESS); 
    }

    cout << "The data has been read from " << filename << endl;
    infile.close();
}

const IVPInfo JsonInfo::GetIVPInfo() const {
    // Get the method label and method name
    int MethodLabel = _root["Method_Label"].asInt();
    string MethodName;
    if (MethodLabel == 1 || MethodLabel == 2 || MethodLabel == 3)  {
        MethodName = _root["Method_Name"][MethodLabel-1].asString();
    } else {
        cerr << "Error in JsonInfo::GetIVPInfo: the method label is invalid" << endl;
        exit(EXIT_SUCCESS);
    }
    // Get the value of ivp
    int p = _root["p"].asInt();
    double TimeStep = _root["Time_Step"].asDouble();
    int CaseLabel = _root["Case_Label"].asInt();
    double TotalTime, mu;
    Array InitialValue;
    switch (CaseLabel) {
        case 1: {
            TotalTime = _root["Case_Description"]["Case_1"]["Total_Time"].asDouble();
            mu = _root["Case_Description"]["Case_1"]["mu"].asDouble();
            int size = _root["Case_Description"]["Case_1"]["Initial_Value"].size();
            InitialValue.resize(size);
            for (int i = 0; i < size; ++i) {
                InitialValue[i] = _root["Case_Description"]["Case_1"]["Initial_Value"][i].asDouble();
            }
            break;
        }
        case 2: {
            TotalTime = _root["Case_Description"]["Case_2"]["Total_Time"].asDouble();
            mu = _root["Case_Description"]["Case_2"]["mu"].asDouble();
            int size = _root["Case_Description"]["Case_2"]["Initial_Value"].size();
            InitialValue.resize(size);
            for (int i = 0; i < size; ++i) {
                InitialValue[i] = _root["Case_Description"]["Case_2"]["Initial_Value"][i].asDouble();
            }
            break;
        }
        default:{
            cerr << "Error in JsonInfo::IVPInfo: the case label is invalid" << endl;
            exit(EXIT_SUCCESS);
            break;
        }
    }
    // Check the validity of the json information
    CheckJsonInfo(p, MethodLabel);
    return IVPInfo(MethodName, p, mu, TimeStep, TotalTime, InitialValue, Example_func);
}

/// @brief Check the validity of the json information
/// @param p 
/// @param MethodName 
void JsonInfo::CheckJsonInfo(const int p, const int MethodLabel) const {
    switch (MethodLabel) {
        case 1:
            if (p < 1 || p > 4) {
                cerr << "Error in JsonInfo::CheckJsonInfo: the value of p is invalid for ABMs" << endl;
                exit(EXIT_SUCCESS);
            }
            break;
        case 2:
            if (p < 2 || p > 5) {
                cerr << "Error in JsonInfo::CheckJsonInfo: the value of p is invalid for AMMs" << endl;
                exit(EXIT_SUCCESS);
            }
            break;
        case 3:
            if (p < 1 || p > 4) {
                cerr << "Error in JsonInfo::CheckJsonInfo: the value of p is invalid for BDFs" << endl;
                exit(EXIT_SUCCESS);
            }
            break;
        default:
            cerr << "Error in JsonInfo::CheckJsonInfo: the method label is invalid" << endl;
            exit(EXIT_SUCCESS);
            break;
    }
}

/// @brief Print the necessary information into terminal
void JsonInfo::PrintJsonInfo(const IVPInfo& IVPInfo) const {
    cout << "=============================================================" << endl;
    cout << "Method Name: " << IVPInfo.MethodName() << endl;
    cout << "p: " << IVPInfo.p() << endl;
    cout << "mu: " << IVPInfo.mu() << endl;
    cout << "Time Step: " << IVPInfo.TimeStep() << endl;
    cout << "Total Time: " << IVPInfo.TotalTime() << endl;
    cout << "Initial Value: ";
    cout << "[";
    for (int i = 0; i < IVPInfo.InitialValue().size(); ++i) {
        cout << setprecision(10) << IVPInfo.InitialValue()[i];
        if (i != IVPInfo.InitialValue().size() - 1) cout << ", ";
    }
    cout << "]" <<endl;
    cout << "=============================================================" << endl;
}
