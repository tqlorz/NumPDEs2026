/**
 * @file          Interpolation.cpp
 * @author        zsh945
 * @date          2026-03-30
 */

#include "Interpolation.hpp"

vector<double> LinearInterpolation(const double Point_x, const double LeftPoint_x, const double RightPoint_x) {
    double coef_L = (RightPoint_x - Point_x) / (RightPoint_x - LeftPoint_x);
    double coef_R = (Point_x - LeftPoint_x) / (RightPoint_x - LeftPoint_x);
    return {coef_L, coef_R};
}

vector<double> LinearInterpolation(const double Point_x, const double Point_y,
                                    const double LeftPoint_x, const double LeftPoint_y,
                                    const double RightPoint_x, const double RightPoint_y) {
    double r_L = DISTANCE_2D(Point_x, Point_y, LeftPoint_x, LeftPoint_y);
    double r_R = DISTANCE_2D(Point_x, Point_y, RightPoint_x, RightPoint_y);
    double r_LR = r_L + r_R;
    double coef_L = r_R / r_LR;
    double coef_R = r_L / r_LR;
    return {coef_L, coef_R};
}

vector<double> RectangleInterpolation(const double Point_x, const double Point_y,
                                    const Rectangle& rectangle) {
    Rectangle LeftDownRectangle(rectangle.corner_x(), rectangle.corner_y(), 
                                Point_x - rectangle.corner_x(), Point_y - rectangle.corner_y());
    Rectangle LeftUpRectangle(rectangle.corner_x(), Point_y, 
                                Point_x - rectangle.corner_x(),  (rectangle.corner_y() + rectangle.height()) - Point_y);
    Rectangle RightDownRectangle(Point_x, rectangle.corner_y(), 
                                (rectangle.corner_x() + rectangle.width()) - Point_x, Point_y - rectangle.corner_y());
    Rectangle RightUpRectangle(Point_x, Point_y, 
                                (rectangle.corner_x() + rectangle.width()) - Point_x, (rectangle.corner_y() + rectangle.height()) - Point_y);
    double TotalArea = rectangle.area();
    double coef_LD = RightUpRectangle.area() / TotalArea;
    double coef_LU = RightDownRectangle.area() / TotalArea;
    double coef_RD = LeftUpRectangle.area() / TotalArea;
    double coef_RU = LeftDownRectangle.area() / TotalArea;
    return {coef_LD, coef_LU, coef_RD, coef_RU};
}

vector<double> TriangleInterpolation(const double Point_x, const double Point_y,
                                    const Triangle& triangle) {
    Triangle Triangle_V1(Point_x, Point_y, triangle.vertex2_x(), triangle.vertex2_y(), triangle.vertex3_x(), triangle.vertex3_y());
    Triangle Triangle_V2(Point_x, Point_y, triangle.vertex3_x(), triangle.vertex3_y(), triangle.vertex1_x(), triangle.vertex1_y());
    Triangle Triangle_V3(Point_x, Point_y, triangle.vertex1_x(), triangle.vertex1_y(), triangle.vertex2_x(), triangle.vertex2_y());
    double TotalArea = triangle.area();
    double coef_V1 = Triangle_V1.area() / TotalArea;
    double coef_V2 = Triangle_V2.area() / TotalArea;
    double coef_V3 = Triangle_V3.area() / TotalArea;
    return {coef_V1, coef_V2, coef_V3};
}