/**
 * @file vect4.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#pragma once


/**
 * @brief vec4 class with 4 elements of type T (e.g. mat33<std::complex<float>>)
 * @tparam T 
 */
template <typename T>
class vec4 {
  public:
    vec4()  = default;
    ~vec4() = default;

    inline const T &operator[](int i) const { return this->data[i]; }
    inline       T &operator[](int i)       { return this->data[i]; }

  private:
    T data[4];
};