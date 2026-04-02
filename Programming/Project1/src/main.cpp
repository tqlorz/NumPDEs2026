/**
 * @file          main.cpp
 * @author        zsh945
 * @date          2026-03-25
 */

#include <cmath>
#include "ParameterCheck.hpp"
#include "FileIO.hpp"
#include "Solver.hpp"

using namespace std;

class Example_1: public BivariateFunction {
public:
    double operator()(const double x, const double y) const {
        return exp(y+sin(x));
    }
};

class Example_2: public BivariateFunction {
public:
    double operator()(const double x, const double y) const {
        return x*(1-x)*y*(1-y);
    }
};

class Example_3: public BivariateFunction {
public:
    double operator()(const double x, const double y) const {
        return sin(x) + sin(y);
    }
};

int main(int argc, char *argv[]) {
    // Read data from json file
    JsonInfo jsonInfo("template.json");
    TopologyInfo topologyInfo = jsonInfo.GetTopologyInfo();
    int FuntionLabel = jsonInfo.GetFunctionLabel();
    jsonInfo.PrintJsonInfo(FuntionLabel);
    // Check the parameters and generate the mesh
    ParameterCheck ParameterCheck;
    ParameterCheck.Check(topologyInfo);
    // Solve the problem
    Example_1 u_1;
    Example_2 u_2;
    Example_3 u_3;
    Solver solver(ParameterCheck.ConnectedMesh());

    switch (FuntionLabel) {
        case 1: {
            solver.Solve(topologyInfo, u_1);
            solver.PrintResultFile(topologyInfo, u_1);
            break;
        }
        case 2: {
            solver.Solve(topologyInfo, u_2);
            solver.PrintResultFile(topologyInfo, u_2);
            break;
        }
        case 3: {
            solver.Solve(topologyInfo, u_3);
            solver.PrintResultFile(topologyInfo, u_3);
            break;
        }
    }
    
    // if (topologyInfo.n() <= 16) {
    //     cout << "Connected Mesh:" << endl;
    //     PrintTensor(solver.ConnectedMesh());
    //     cout << "Boundary Label:" << endl;
    //     PrintTensor(solver.BoundaryLabel());
    //     cout << "Node Order Mesh:" << endl;
    //     PrintTensor(solver.NodeOrderMesh());
    // }

    return 0;
}