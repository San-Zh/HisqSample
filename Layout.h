/**
 * @file Layout.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include <cassert>

/**
 * @brief 
 * 
 */
enum EvenOdd_t { EVEN, ODD, FULL };

/**
 * @brief 
 * 
 */
struct Layout {
    int X, Y, Z, T;
    int volFull, volHalf;
    Layout() = default;
    Layout(const int _X, const int _Y, const int _Z, const int _T) { resize(_X, _Y, _Z, _T); }
    Layout(const int L) { resize(L, L, L, L); }
    Layout(const Layout &other) { *this = other; } // copy constructor

    int       &vol() { return this->volFull; }
    const int &vol() const { return this->volFull; }
    int       &volh() { return this->volHalf; }
    const int &volh() const { return this->volHalf; }

    Layout &operator=(const Layout &other)
    {
        this->X       = other.X;
        this->Y       = other.Y;
        this->Z       = other.Z;
        this->T       = other.T;
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
        assert(X % 2 == 0 && Y % 2 == 0 && Z % 2 == 0 && T % 2 == 0);

        this->volFull = _X * _Y * _Z * _T;
        this->volHalf = _X * _Y * _Z * _T / 2;
    }

    void resize(const int L) { this->resize(L, L, L, L); }
};


static inline int evenoddIndex(const int x, const int y, const int z, const int t, const Layout &lat) { return (x + lat.X * (y + lat.Y * (z + lat.Z * t))) / 2 + (x + y + z + t) % 2 * lat.volHalf; }

static inline int linearIndex(const int x, const int y, const int z, const int t, const Layout &lat) { return (x + lat.X * (y + lat.Y * (z + lat.Z * t))); }

/**
 * @brief Global variable for the layout of the lattice.
 * 
 */
extern Layout layout;