/**
 * @file vec3.h
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

// clang-format off
template <typename T> class vec3;
// template <typename T>  class vec3<std::complex<T>>;
// clang-format on

template <typename T>
class vec3 {
  public:
    vec3()  = default;
    ~vec3() = default;
    // clang-format off

    inline       T &operator[](int i)       { return this->data[i]; }
    inline const T &operator[](int i) const { return this->data[i]; }

    // clang-format on
    // inline void zero()
    // {
    //     this->data[0] = 0.0;
    //     this->data[1] = 0.0;
    //     this->data[2] = 0.0;
    // }
    // inline void ones()
    // {
    //     this->data[0] = 1.0;
    //     this->data[1] = 1.0;
    //     this->data[2] = 1.0;
    // }
    // template <typename Tp = T>
    // inline vec3 &operator()(const Tp &e0, const T &e1, const T &e2)
    // {
    //     this->data[0] = e0;
    //     this->data[1] = e1;
    //     this->data[2] = e2;
    // }
    // inline vec3<T> &operator=(const vec3<T> &b)
    // {
    //     this->data[0] = b.data[0];
    //     this->data[1] = b.data[1];
    //     this->data[2] = b.data[2];
    //     return *this;
    // }
    vec3<T> &operator+=(const vec3<T> &b)
    {
        this->data[0] += b.data[0];
        this->data[1] += b.data[1];
        this->data[2] += b.data[2];
        return *this;
    }
    vec3<T> &operator-=(const vec3<T> &b)
    {
        this->data[0] -= b.data[0];
        this->data[1] -= b.data[1];
        this->data[2] -= b.data[2];
        return *this;
    }
    vec3<T> &operator*=(const vec3<T> &b)
    {
        this->data[0] *= b.data[0];
        this->data[1] *= b.data[1];
        this->data[2] *= b.data[2];
        return *this;
    }
    vec3<T> &operator*=(const T &b)
    {
        this->data[0] *= b;
        this->data[1] *= b;
        this->data[2] *= b;
        return *this;
    }

  private:
    T data[3];
};


template <typename T>
vec3<T> operator-(const vec3<T> &a, const vec3<T> &b)
{
    vec3<T> res;
    res[0] = a[0] - b[0];
    res[1] = a[1] - b[1];
    res[2] = a[2] - b[2];
    return res;
}

template <typename T>
T innerProduct(const vec3<T> &a, const vec3<T> &b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

template <typename T>
std::complex<double> innerProduct(const vec3<std::complex<T>> &a, const vec3<std::complex<T>> &b)
{
    throw std::runtime_error("This function To Be Checked");
    return a[0] * std::conj(b[0]) + a[1] * std::conj(b[1]) + a[2] * std::conj(b[2]);
}

template <typename T>
double norm2(const vec3<std::complex<T>> &a)
{
    return a[0].real() * a[0].real() + a[0].imag() * a[0].imag() + a[1].real() * a[1].real() + a[1].imag() * a[1].imag() + a[2].real() * a[2].real() + a[2].imag() * a[2].imag();
}

// template <>
// double innerProduct<std::complex<double>>(const vec3<std::complex<double>> &a, const vec3<std::complex<double>> &b)
// {
//     return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
// }

// template <>
// float innerProduct<std::complex<float>>(const vec3<std::complex<float>> &a, const vec3<std::complex<float>> &b)
// {
//     return a[0] * std::conj(b[0]) + a[1] * std::conj(b[1]) + a[2] * std::conj(b[2]);
// }