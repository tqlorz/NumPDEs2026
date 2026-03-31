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
        // return x*(1-x)*y*(1-y);
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

        // cout << "Vector U:" << endl;
        // for (int i = 0; i < solver.U().size(); i++) {
        //     cout << NodeOrderToMesh(i).first << " " << NodeOrderToMesh(i).second << ": " << solver.U()(i) << endl;  
        // }
        // cout << endl;
    }

    // Rectangle test = Rectangle(0, 0, 1, 1);
    // vector<double> record = RectangleInterpolation(0.2,0.3, test);
    // for (int i = 0; i < record.size(); i++) {
    //     cout << "Coefficient " << i << ": " << record[i] << endl;
    // }

    // Triangle test_triangle = Triangle(0, 0, 1, 0, 0, 1);
    // vector<double> record_triangle = TriangleInterpolation(0.6, 0.7, test_triangle);
    // for (int i = 0; i < record_triangle.size(); i++) {
    //     cout << "Coefficient " << i << ": " << record_triangle[i] << endl;
    // }
    

    return 0;
}