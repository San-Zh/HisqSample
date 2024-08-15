/**
 * @file HisqDslash.h
 * @author your name (you@domain.com)
 * @brief covirant SU3 stencil implementation
 * @version 0.1
 * @date 2024-08-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <complex>
#include <cassert>
#include <iostream>
#include <omp.h>
#include "vec3.h"
#include "mat33.h"

struct Layout {
    int X, Y, Z, T;
    int volFull, volHalf;
    Layout() = default;
    Layout(const int _X, const int _Y, const int _Z, const int _T) { resize(_X, _Y, _Z, _T); }
    Layout(const int L) { resize(L, L, L, L); }
    Layout(const Layout &other) { *this = other; } // copy constructor

    int &vol() { return this->volFull; }
    const int &vol() const { return this->volFull; }
    int &volh() { return this->volHalf; }
    const int &volh() const { return this->volHalf; }

    Layout &operator=(const Layout &other)
    {
        this->X = other.X;
        this->Y = other.Y;
        this->Z = other.Z;
        this->T = other.T;
        this->volFull = other.volFull;
        this->volHalf = other.volHalf;
        return *this;
    }

    void resize(const int _X, const int _Y, const int _Z, const int _T)
    {
        this->X = _X;
        this->Y = _Y;
        this->Z = _Z;
        this->T = _T;
        this->volFull = _X * _Y * _Z * _T;
        this->volHalf = _X * _Y * _Z * _T / 2;
        assert(volFull % 2 == 0);
        assert(X % 2 == 0);
    }

    void resize(const int L) { this->resize(L, L, L, L); }
};

extern Layout layout;

static inline int evenoddIndex(const int x, const int y, const int z, const int t, const Layout &lat)
{
    return (x + lat.X * (y + lat.Y * (z + lat.Z * t))) / 2 + (x + y + z + t) % 2 * lat.volHalf;
}

static inline int linearIndex(const int x, const int y, const int z, const int t, const Layout &lat)
{
    return (x + lat.X * (y + lat.Y * (z + lat.Z * t)));
}

/**
 * @brief Field base class for public API
 * 
 * @tparam T 
 */
template <typename T>
class Field {
  public:
    Field(const Layout &lat) : _lat(lat) { this->_p = new T[_lat.vol()]; }
    Field() : Field(layout) {}
    virtual ~Field() { delete[] this->_p; }

    virtual void zero() {}
    virtual void random() {}

    virtual void layout(const Layout &lat)
    {
        this->_lat = lat;
        if (this->_p == nullptr) { delete[] this->_p; }
        this->_p = new T[_lat.vol()];
    }

    // clang-format off
    virtual const Layout &layout()const { return this->_lat; }
    virtual       Layout &layout()      { return this->_lat; }

    virtual const T &data(int i) const { return this->_p[i]; }
    virtual       T &data(int i) { return this->_p[i]; }

    virtual const T &operator()(int i) const { return this->data(i); }
    virtual       T &operator()(int i) { return this->data(i); }

    virtual const T &operator()(int x, int y, int z, int t) const { return this->_p[evenoddIndex(x, y, z, t, this->layout())]; }
    virtual       T &operator()(int x, int y, int z, int t)       { return this->_p[evenoddIndex(x, y, z, t, this->layout())]; }
    // clang-format on
  private:
    Layout _lat;
    T *_p = nullptr;
};

template <typename T>
class SpinorField : public Field<vec3<std::complex<T>>> {
  public:
    using Field<vec3<std::complex<T>>>::Field; // inherit constructor

    void random()
    {
        T _rn = 1.0 / RAND_MAX;
        for (int i = 0; i < layout.vol(); i++) {
            this->data(i)[0] = std::complex<T>((T) rand() * _rn, (T) rand() * _rn);
            this->data(i)[1] = std::complex<T>((T) rand() * _rn, (T) rand() * _rn);
            this->data(i)[2] = std::complex<T>((T) rand() * _rn, (T) rand() * _rn);
        }
    }
};

template <typename T>
class SU3Field : public Field<mat33<std::complex<T>>> {
  public:
    using Field<mat33<std::complex<T>>>::Field; // inherit constructor

    void random()
    {
        T _rn = 1.0 / RAND_MAX;
        for (int i = 0; i < layout.vol(); i++) {
            this->data(i)[0][0] = std::complex<T>((T) rand() * _rn, (T) rand() * _rn);
            this->data(i)[0][1] = std::complex<T>((T) rand() * _rn, (T) rand() * _rn);
            this->data(i)[0][2] = std::complex<T>((T) rand() * _rn, (T) rand() * _rn);
            this->data(i)[1][0] = std::complex<T>((T) rand() * _rn, (T) rand() * _rn);
            this->data(i)[1][1] = std::complex<T>((T) rand() * _rn, (T) rand() * _rn);
            this->data(i)[1][2] = std::complex<T>((T) rand() * _rn, (T) rand() * _rn);
            this->data(i)[2][0] = std::complex<T>((T) rand() * _rn, (T) rand() * _rn);
            this->data(i)[2][1] = std::complex<T>((T) rand() * _rn, (T) rand() * _rn);
            this->data(i)[2][2] = std::complex<T>((T) rand() * _rn, (T) rand() * _rn);
        }
    }
};
