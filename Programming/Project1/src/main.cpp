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

class function_1: public BivariateFunction {
public:
    double operator()(const double x, const double y) const {
        return exp(y+sin(x));
    }
};

int main(int argc, char *argv[]) {
    // Read data from json file
    JsonInfo jsonInfo("template.json");
    jsonInfo.PrintJsonInfo();
    TopologyInfo topologyInfo = jsonInfo.GetTopologyInfo();
    // Check the parameters and generate the mesh
    ParameterCheck ParameterCheck;
    ParameterCheck.Check(topologyInfo);
    // Solve the problem
    function_1 u;
    Solver solver(ParameterCheck.ConnectedMesh());
    solver.Solve(topologyInfo, u);
    solver.PrintResultFile(topologyInfo, u);
    
    if (topologyInfo.n() <= 16) {
        // cout << "Connected Mesh:" << endl;
        // PrintTensor(solver.ConnectedMesh());
        cout << "Boundary Label:" << endl;
        PrintTensor(solver.BoundaryLabel());
        cout << "Node Order Mesh:" << endl;
        PrintTensor(solver.NodeOrderMesh());
        
        // cout << "Node Order to Mesh:" << endl;
        Tensor<pair<int, int>, 1> NodeOrderToMesh = solver.NodeOrderToMesh();
        // for (int i = 0; i < NodeOrderToMesh.size(); i++) {
        //     cout << "Node " << i << ": (" << NodeOrderToMesh(i).first << ", " << NodeOrderToMesh(i).second << ")" << endl;
        // }

        
        // cout << "Vector F:" << endl;
        // for (int i = 0; i < solver.F().size(); i++) {
        //     cout << NodeOrderToMesh(i).first << " " << NodeOrderToMesh(i).second << ": " << solver.F()(i) << endl;  
        // }
        // cout << endl;

        cout << "Vector U:" << endl;
        for (int i = 0; i < solver.U().size(); i++) {
            cout << NodeOrderToMesh(i).first << " " << NodeOrderToMesh(i).second << ": " << solver.U()(i) << endl;  
        }
        cout << endl;
    }

    return 0;
}