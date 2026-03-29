/**
 * @file          TopologyInfo.hpp
 * @author        zsh945
 * @date          2026-03-27
 */

# ifndef _TOPOLOGYINFO_
# define _TOPOLOGYINFO_

#include "Shape.hpp"
#include <vector>
#include <string>

using namespace std;

/// @brief Class for storing the topology information of the mesh
class TopologyInfo {
private:
    int _n;
    Rectangle _Rectangle;
    Circle _Circle;
    vector<string> _RectangleBoundaryCondition;
    string _CircleBoundaryCondition;
public:
    TopologyInfo(int n, const Rectangle& Rectangle, const Circle& Circle, 
                const vector<string>& RectangleBoundaryCondition, const string& CircleBoundaryCondition)
                : _n(n), _Rectangle(Rectangle), _Circle(Circle), 
                  _RectangleBoundaryCondition(RectangleBoundaryCondition), _CircleBoundaryCondition(CircleBoundaryCondition) {}
    int n() const {return _n;}
    const Rectangle& rectangle() const {return _Rectangle;}
    const Circle& circle() const {return _Circle;}
    const vector<string>& rectangle_boundary_condition() const {return _RectangleBoundaryCondition;}
    const string& circle_boundary_condition() const {return _CircleBoundaryCondition;}
};

# endif