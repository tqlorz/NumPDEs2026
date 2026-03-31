/**
 * @file          Shape.hpp
 * @author        zsh945
 * @date          2026-03-26
 * @brief         This file contains the declaration of the Shape class, which is used to represent the shape of the domain.
 */

#ifndef _SHAPE_
#define _SHAPE_

#include <iostream>
#include <cmath>

using namespace std;

class Shape {
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
};

class Circle : public Shape {
private:
    double _Center_x, _Center_y;
    double _Radius;
public:
    Circle(const double x, const double y, const double r) : _Center_x(x), _Center_y(y), _Radius(r) {}

    double area() const {return M_PI * _Radius * _Radius;}
    double perimeter() const {return 2 * M_PI * _Radius;}
    double center_x() const { return _Center_x;}
    double center_y() const { return _Center_y;}
    double radius() const { return _Radius;}
};

class Rectangle : public Shape {
private:
    double _Corner_x, _Corner_y;      // The coordinates of the bottom-left corner of the rectangle
    double _Width, _Height;
public:
    Rectangle(const double x, const double y, const double w, const double h) : _Corner_x(x), _Corner_y(y), _Width(w), _Height(h) {}

    double area() const {return _Width * _Height; }
    double perimeter() const {return 2 * (_Width + _Height); }
    double corner_x() const { return _Corner_x;}
    double corner_y() const { return _Corner_y;}
    double width() const { return _Width; }
    double height() const { return _Height; }
};

class Triangle : public Shape {
private:
    double _Vertex1_x, _Vertex1_y;
    double _Vertex2_x, _Vertex2_y;
    double _Vertex3_x, _Vertex3_y;
public:
    Triangle(const double x1, const double y1, const double x2, const double y2, const double x3, const double y3) : 
    _Vertex1_x(x1), _Vertex1_y(y1), _Vertex2_x(x2), _Vertex2_y(y2), _Vertex3_x(x3), _Vertex3_y(y3) {}   
    double area() const {
        return 0.5 * ((_Vertex2_x - _Vertex1_x) * (_Vertex3_y - _Vertex1_y) 
                    - (_Vertex3_x - _Vertex1_x) * (_Vertex2_y - _Vertex1_y));
    }
    double perimeter() const {
        double side1 = sqrt(pow(_Vertex2_x - _Vertex1_x, 2) + pow(_Vertex2_y - _Vertex1_y, 2));
        double side2 = sqrt(pow(_Vertex3_x - _Vertex2_x, 2) + pow(_Vertex3_y - _Vertex2_y, 2));
        double side3 = sqrt(pow(_Vertex1_x - _Vertex3_x, 2) + pow(_Vertex1_y - _Vertex3_y, 2));
        return side1 + side2 + side3;
    }
    double vertex1_x() const { return _Vertex1_x;}
    double vertex1_y() const { return _Vertex1_y;}
    double vertex2_x() const { return _Vertex2_x;}
    double vertex2_y() const { return _Vertex2_y;}
    double vertex3_x() const { return _Vertex3_x;}
    double vertex3_y() const { return _Vertex3_y;}
};  

#endif