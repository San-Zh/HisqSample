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
#include "vec4.h"
#include "mat33.h"
#include "Layout.h"

/**
 * @brief Field base class for public API
 * 
 * @tparam T 
 */
template <typename T>
class Field {
  public:
    Field(const Layout &lat) : _lat(lat) { this->_p = new T[_lat.vol()]; }
    // Field() : _lat(layout) { this->_p = new T[_lat.vol()]; }
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
    virtual       T &data(int i)       { return this->_p[i]; }

    virtual const T &operator()(int i) const { return this->_p[i]; }
    virtual       T &operator()(int i)       { return this->_p[i]; }

    virtual const T &lineardata(int x, int y, int z, int t) const { return this->_p[linearIndex(x, y, z, y, this->layout())]; }
    virtual       T &lineradata(int x, int y, int z, int t)       { return this->_p[linearIndex(x, y, z, y, this->layout())]; }

    virtual const T &operator()(int x, int y, int z, int t) const { return this->_p[evenoddIndex(x, y, z, t, this->layout())]; }
    virtual       T &operator()(int x, int y, int z, int t)       { return this->_p[evenoddIndex(x, y, z, t, this->layout())]; }
    // clang-format on
  private:
    Layout _lat;
    T     *_p = nullptr;
};

/**
 * @brief SpinorField class for spinor field
 * 
 * @tparam T 
 */
template <typename T>
class SpinorField : public Field<vec3<std::complex<T>>> {
  public:
    typedef vec3<std::complex<T>> DataType;
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

/**
 * @brief 
 * 
 * @tparam T 
 */
template <typename T>
class SU3Field : public Field<mat33<std::complex<T>>> {
  public:
    typedef mat33<std::complex<T>> DataType;

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

/**
 * @brief GaugeLinkField class for gauge link field
 * one GaugeLinkField with four components of SU3Field, is  U1, U2, U3, vec4, each with 4 components of su3 matrix (mat33)
 * 
 * @tparam T 
 */
template <typename T>
class GaugeField {
  public:
    typedef SU3Field<T> DataType;

    // clang-format off
    GaugeField(const Layout &lat) { for (int mu = 0; mu < 4; mu++) { _U[mu] = new SU3Field<T>(lat); } }
    ~GaugeField() { for (int mu = 0; mu < 4; mu++) { delete _U[mu]; } }
    void random() { for (int mu = 0; mu < 4; mu++) { _U[mu]->random(); } }

    const Layout &layout() const { return _U[0]->layout(); }
          Layout &layout()       { return _U[0]->layout(); }

    const DataType &operator[](int mu) const { return *_U[mu]; }
          DataType &operator[](int mu)       { return *_U[mu]; }
    // clang-format on

  private:
    DataType *_U[4]; // gauge field
};



/**
 * @brief GaugeFieldNew class for GaugeLinkField
 * @note typedef vec4<mat33<std::complex<T>>>  DataType; \n
 * for each site, there are four su3 matrices, each with 3x3 components
 * 
 * @tparam T 
 */
template <typename T>
class GaugeFieldNew : public Field<vec4<mat33<std::complex<T>>>> {
  public:
    typedef vec4<mat33<std::complex<T>>> DataType;
    using Field<DataType>::Field; // inherit constructor

    void random()
    {
        T _rn = 1.0 / RAND_MAX;
        for (int i = 0; i < layout.vol(); i++) {
            for (int mu = 0; mu < 4; mu++) {
                for (int r = 0; r < 3; r++) {
                    for (int c = 0; c < 3; c++) { this->data(i)[mu][r][c] = std::complex<T>((T) rand() * _rn, (T) rand() * _rn); }
                }
            }
        }
    }
};
