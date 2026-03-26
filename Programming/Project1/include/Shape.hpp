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

#endif