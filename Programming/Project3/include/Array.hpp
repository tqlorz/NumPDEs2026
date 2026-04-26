/**
 * @file          Array.hpp
 * @author        zsh945
 * @date          2026-04-25
 */

#ifndef _ARRAY_
#define _ARRAY_

#include <cmath>
#include <vector>
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <utility>

class Array {
private:
    std::vector<double> _data;
public:
    Array() = default;
    explicit Array(size_t n) : _data(n, 0.0) {}
    Array(size_t n, double val) : _data(n, val) {}
    Array(std::initializer_list<double> list) : _data(list) {}

    template<typename Iter>
    Array(Iter first, Iter last) : _data(first, last) {}

    // Interface
    void resize(size_t n) { _data.resize(n); }
    size_t size() const { return _data.size(); }
    double* data() { return _data.data(); }
    const double* data() const { return _data.data(); }

    double& operator[](size_t i) { return _data[i]; }
    const double& operator[](size_t i) const { return _data[i]; }

    // Iterators
    auto begin() { return _data.begin(); }
    auto end()   { return _data.end(); }
    auto begin() const { return _data.begin(); }
    auto end()   const { return _data.end(); }

    // fill
    void fill(double val) { std::fill(begin(), end(), val); }

    // Calculate with self
    Array& operator+=(const Array& rhs) {
        assert(size() == rhs.size());
        for (size_t i = 0; i < size(); ++i) _data[i] += rhs[i];
        return *this;
    }
    Array& operator-=(const Array& rhs) {
        assert(size() == rhs.size());
        for (size_t i = 0; i < size(); ++i) _data[i] -= rhs[i];
        return *this;
    }
    Array& operator*=(const Array& rhs) {
        assert(size() == rhs.size());
        for (size_t i = 0; i < size(); ++i) _data[i] *= rhs[i];
        return *this;
    }
    Array& operator/=(const Array& rhs) {
        assert(size() == rhs.size());
        for (size_t i = 0; i < size(); ++i) _data[i] /= rhs[i];
        return *this;
    }
    Array& operator*=(double s) {
        for (double& x : _data) x *= s;
        return *this;
    }
    Array& operator/=(double s) {
        for (double& x : _data) x /= s;
        return *this;
    }
};

// Calculate with arrays
inline Array operator+(const Array& a, const Array& b) {
    assert(a.size() == b.size());
    Array res(a.size());
    for (size_t i = 0; i < a.size(); ++i) res[i] = a[i] + b[i];
    return res;
}
inline Array operator-(const Array& a, const Array& b) {
    assert(a.size() == b.size());
    Array res(a.size());
    for (size_t i = 0; i < a.size(); ++i) res[i] = a[i] - b[i];
    return res;
}
inline Array operator*(const Array& a, const Array& b) {
    assert(a.size() == b.size());
    Array res(a.size());
    for (size_t i = 0; i < a.size(); ++i) res[i] = a[i] * b[i];
    return res;
}
inline Array operator/(const Array& a, const Array& b) {
    assert(a.size() == b.size());
    Array res(a.size());
    for (size_t i = 0; i < a.size(); ++i) res[i] = a[i] / b[i];
    return res;
}

// Calculate with scalars
inline Array operator+(const Array& a, double s) {
    Array res(a.size());
    for (size_t i = 0; i < a.size(); ++i) res[i] = a[i] + s;
    return res;
}
inline Array operator+(double s, const Array& a) { return a + s; }

inline Array operator-(const Array& a, double s) {
    Array res(a.size());
    for (size_t i = 0; i < a.size(); ++i) res[i] = a[i] - s;
    return res;
}
inline Array operator-(double s, const Array& a) {
    Array res(a.size());
    for (size_t i = 0; i < a.size(); ++i) res[i] = s - a[i];
    return res;
}

inline Array operator*(const Array& a, double s) {
    Array res(a.size());
    for (size_t i = 0; i < a.size(); ++i) res[i] = a[i] * s;
    return res;
}
inline Array operator*(double s, const Array& a) { return a * s; }

inline Array operator/(const Array& a, double s) {
    Array res(a.size());
    for (size_t i = 0; i < a.size(); ++i) res[i] = a[i] / s;
    return res;
}
inline Array operator/(double s, const Array& a) {
    Array res(a.size());
    for (size_t i = 0; i < a.size(); ++i) res[i] = s / a[i];
    return res;
}

// Calculate functions (sqrt, abs)
inline Array sqrt(const Array& a) {
    Array res(a.size());
    for (size_t i = 0; i < a.size(); ++i) res[i] = std::sqrt(a[i]);
    return res;
}
inline Array abs(const Array& a) {
    Array res(a.size());
    for (size_t i = 0; i < a.size(); ++i) res[i] = std::abs(a[i]);
    return res;
}
inline double sum(const Array& a) {
    double res = 0.0;
    for (size_t i = 0; i < a.size(); ++i) res += a[i];
    return res;
}

#endif