/**
 * @file          main.cpp
 * @author        zsh945
 * @date          2026-03-25
 */

#include <iostream>
#include "Tensor.hpp"
#include "ParameterCheck.hpp"
#include "FileIO.hpp"
#include "ConnectedRegion.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    JsonInfo jsonInfo("template.json");
    jsonInfo.PrintJsonInfo();
    int n = jsonInfo.n();
    Rectangle Rectangle(0, 0,jsonInfo.rectangle_width(), jsonInfo.rectangle_height());
    Circle Circle(jsonInfo.circle_center_x(), jsonInfo.circle_center_y(), jsonInfo.circle_radius());
    ParameterCheck ParameterCheck(n, Rectangle, Circle, jsonInfo.rectangle_boundary_condition(), jsonInfo.circle_boundary_condition());
    ParameterCheck.Check();

    return 0;
}