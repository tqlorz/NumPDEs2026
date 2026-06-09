/**
 * @file TimeIntegrator.cpp
 * @author zsh945
 * @date 2026-05-01
 */

#include "TimeIntegrator.hpp"

/// @brief Print the result to file
/// @param filename 
/// @param u 
/// @param t 
void PrintResultFile(const string& filename, const Array& u, const double t){
    if (t == 0) {
        ofstream initFile(filename, ios::trunc);
        initFile.close();
    }
    ofstream resultFile(filename, ios::app);
    if (!resultFile.is_open()) {
        cerr << "Failed to open the result file!" << endl;
        return;
    }
    resultFile << t << " ";
    for (size_t i = 0; i < u.size(); i++){
        resultFile << u[i];
        if (i != u.size() - 1) resultFile << " ";
    }
    resultFile << endl;
    resultFile.close();
}