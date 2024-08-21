/**
 * @file matt33.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <complex>
#include "vec3.h"

template <typename T>
class mat33;

template <typename T>
class mat33 {
  public:
    mat33()  = default;
    ~mat33() = default;

    // clang-format off

    inline const vec3<T> &operator[](int i) const { return this->data[i]; }
    inline       vec3<T> &operator[](int i)       { return this->data[i]; }

    inline const mat33<T> &hermitian(const mat33<T> &other) const {
        mat33<T> res;
        res[0][0] = std::conj(other[0][0]);
        res[1][0] = std::conj(other[0][1]);
        res[2][0] = std::conj(other[0][2]);
        res[0][1] = std::conj(other[1][0]);
        res[1][1] = std::conj(other[1][1]);
        res[2][1] = std::conj(other[1][2]);
        res[0][2] = std::conj(other[2][0]);
        res[1][2] = std::conj(other[2][1]);
        res[2][2] = std::conj(other[2][2]);
        return res;
    }
    
    inline mat33<T> &hermitian(const mat33<T> &other){
        mat33<T> res;
        res[0][0] = std::conj(other[0][0]);
        res[1][0] = std::conj(other[0][1]);
        res[2][0] = std::conj(other[0][2]);
        res[0][1] = std::conj(other[1][0]);
        res[1][1] = std::conj(other[1][1]);
        res[2][1] = std::conj(other[1][2]);
        res[0][2] = std::conj(other[2][0]);
        res[1][2] = std::conj(other[2][1]);
        res[2][2] = std::conj(other[2][2]);
        return res;
    }

    // friend template<Tp> vec3<Tp> maulv(const mat33<T> &A, const vec3<T> &b);
    // friend template<Tp> vec3<Tp> mdagv(const mat33<T> &A, const vec3<T> &b);

    // clang-format on

  private:
    vec3<T> data[3];
};

template <typename T>
mat33<T> outproduct(const vec3<T> &a, const vec3<T> &b)
{
    mat33<T> res;
    res[0][0] = a[0] * b[0];
    res[0][1] = a[0] * b[1];
    res[0][2] = a[0] * b[2];
    res[1][0] = a[1] * b[0];
    res[1][1] = a[1] * b[1];
    res[1][2] = a[1] * b[2];
    res[2][0] = a[2] * b[0];
    res[2][1] = a[2] * b[1];
    res[2][2] = a[2] * b[2];
    return res;
}

/**
 * @brief 
 * 
 * @tparam T 
 * @param A 
 * @param b 
 * @return vec3<T> 
 */
template <typename T>
vec3<T> mul(const mat33<T> &A, const vec3<T> &b)
{
    vec3<T> res;
    res[0] = A[0][0] * b[0] + A[1][0] * b[1] + A[2][0] * b[2];
    res[1] = A[0][1] * b[0] + A[1][1] * b[1] + A[2][1] * b[2];
    res[2] = A[0][2] * b[0] + A[1][2] * b[1] + A[2][2] * b[2];
    return res;
}

template <typename T>
vec3<T> operator*(const vec3<T> &A, const mat33<T> &b)
{
    return mul(A, b);
}

// matrix-vector multiplication with conjugate transpose
template <typename Tp>
vec3<std::complex<Tp>> mdagv(const mat33<std::complex<Tp>> &A, const vec3<std::complex<Tp>> &b)
{
    vec3<std::complex<Tp>> res;
    res[0] = std::conj(std::conj(b[0]) * A[0][0] + std::conj(b[1]) * A[1][0] + std::conj(b[2]) * A[2][0]);
    res[1] = std::conj(std::conj(b[0]) * A[0][1] + std::conj(b[1]) * A[1][1] + std::conj(b[2]) * A[2][1]);
    res[2] = std::conj(std::conj(b[0]) * A[0][2] + std::conj(b[1]) * A[1][2] + std::conj(b[2]) * A[2][2]);
    return res;
}
