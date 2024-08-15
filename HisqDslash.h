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

#include <omp.h>
#include "Field.h"

// #define Debug

#ifndef Debug

template <typename T>
void HisqDslash(SpinorField<T> &spinorOut, const SU3Field<T> &U0, const SpinorField<T> &spinorIn)
{
#pragma omp parallel for
    for (int t = 0; t < layout.T; t++) {
        for (int z = 0; z < layout.Z; z++) {
            for (int y = 0; y < layout.Y; y++) {
                for (int x = 0; x < layout.X; x += 1) {
                    int xf = (x + 1 + layout.X) % layout.X;
                    int xb = (x - 1 + layout.X) % layout.X;
                    spinorOut(x, y, z, t) += mul(U0(x, y, z, t), spinorIn(xf, y, z, t)) - mdagv(U0(xb, y, z, t), spinorIn(xb, y, z, t));
                }
            }
        }
    }
}

#else

enum EvenOdd_t { EVEN, ODD, FULL };

template <typename T, EvenOdd_t Leo, EvenOdd_t Reo>
void HisqDslashH(SpinorField<T> &spinorOut, const SU3Field<T> &U0, const SpinorField<T> &spinorIn);

template <typename T>
void HisqDslashH<T, EVEN, EVEN>(SpinorField<T> &spinorOut, const SU3Field<T> &U0, const SpinorField<T> &spinorIn)
{
#pragma omp parallel for
    for (int t = 0; t < layout.T; t++) {
        for (int z = 0; z < layout.Z; z++) {
            for (int y = 0; y < layout.Y; y++) {
                for (int x = 0; x < layout.X; x += 1) {
                    int xf = (x + 1 + layout.X) % layout.X;
                    int xb = (x - 1 + layout.X) % layout.X;
                    spinorOut(x, y, z, t) += mul(U0(x, y, z, t), spinorIn(xf, y, z, t)) - mdagv(U0(xb, y, z, t), spinorIn(xb, y, z, t));
                }
            }
        }
    }
}

template <typename T>
void HisqDslash(SpinorField<T> &spinorOut, const SU3Field<T> &U0, const SpinorField<T> &spinorIn)
{
    HisqDslashH<T, EVEN, EVEN>(spinorOut, U0, spinorIn);
}

#endif

#undef Debug