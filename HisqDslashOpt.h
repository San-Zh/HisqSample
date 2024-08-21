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

/**
 * @brief Implementation of the HisqDslash operator for the given even/odd and full/half spinor fields.
 * 
 * @tparam Leo 
 * @tparam Reo 
 */
template <EvenOdd_t Leo, EvenOdd_t Reo>
struct HisqDslashOptImpl {
    template <typename T>
    static void call(SpinorField<T> &spinorOut, const GaugeField<T> &U, const SpinorField<T> &spinorIn)
    {
        throw std::runtime_error("Error: NO Implemented! Only <EVEN, ODD>, <ODD, EVEN>, and <FULL, FULL> are supported.");
    }

    template <typename T>
    static void call(SpinorField<T> &spinorOut, const GaugeField<T> &U, const GaugeField<T> &Udag, const SpinorField<T> &spinorIn)
    {
        throw std::runtime_error("Error: NO Implemented! Only <EVEN, ODD>, <ODD, EVEN>, and <FULL, FULL> are supported.");
    }

    template <typename T>
    static void call(SpinorField<T> &spinorOut, const GaugeFieldNew<T> &U, const SpinorField<T> &spinorIn)
    {
        throw std::runtime_error("Error: NO Implemented! Only <EVEN, ODD>, <ODD, EVEN>, and <FULL, FULL> are supported.");
    }
};

// #define Debug

#ifndef WITH_BUFFER

template <>
struct HisqDslashOptImpl<FULL, FULL> {
    template <typename T>
    static void call(SpinorField<T> &spinorOut, const GaugeField<T> &U, const SpinorField<T> &spinorIn)
    {
        int xf = 0, xb = 0;
        int yf = 0, yb = 0;
        int zf = 0, zb = 0;
        int tf = 0, tb = 0;

        const int STR = 2;
#pragma omp parallel for private(xf, xb, yf, yb, zf, zb, tf, tb)
        for (int t = 0; t < layout.T; t++) {
            for (int z = 0; z < layout.Z; z++) {
                for (int y = 0; y < layout.Y; y++) {
                    for (int ix = 0; ix < layout.X; ix += STR) {
                        for (int x = ix; x < ix + STR; x++) {
                            xf = (x + 1 + layout.X) % layout.X;
                            xb = (x - 1 + layout.X) % layout.X;
                            yf = (y + 1 + layout.Y) % layout.Y;
                            yb = (y - 1 + layout.Y) % layout.Y;
                            zf = (z + 1 + layout.Z) % layout.Z;
                            zb = (z - 1 + layout.Z) % layout.Z;
                            tf = (t + 1 + layout.T) % layout.T;
                            tb = (t - 1 + layout.T) % layout.T;
                            spinorOut(x, y, z, t) += mul(U[0](x, y, z, t), spinorIn(xf, y, z, t)) - mdagv(U[0](xb, y, z, t), spinorIn(xb, y, z, t));
                            spinorOut(x, y, z, t) += mul(U[1](x, y, z, t), spinorIn(x, yf, z, t)) - mdagv(U[1](x, yb, z, t), spinorIn(x, yb, z, t));
                            spinorOut(x, y, z, t) += mul(U[2](x, y, z, t), spinorIn(x, y, zf, t)) - mdagv(U[2](x, y, zb, t), spinorIn(x, y, zb, t));
                            spinorOut(x, y, z, t) += mul(U[3](x, y, z, t), spinorIn(x, y, z, tf)) - mdagv(U[3](x, y, z, tb), spinorIn(x, y, z, tb));
                        }
                    }
                }
            }
        }
    }

    template <typename T>
    static void call(SpinorField<T> &spinorOut, const GaugeField<T> &U, const GaugeField<T> &Udag, const SpinorField<T> &spinorIn)
    {
        int xf = 0, xb = 0;
        int yf = 0, yb = 0;
        int zf = 0, zb = 0;
        int tf = 0, tb = 0;

        const int STR = 2;
#pragma omp parallel for private(xf, xb, yf, yb, zf, zb, tf, tb)
        for (int t = 0; t < layout.T; t++) {
            for (int z = 0; z < layout.Z; z++) {
                for (int y = 0; y < layout.Y; y++) {
                    for (int ix = 0; ix < layout.X; ix += STR) {
                        for (int x = ix; x < ix + STR; x++) {
                            xf = (x + 1 + layout.X) % layout.X;
                            xb = (x - 1 + layout.X) % layout.X;
                            yf = (y + 1 + layout.Y) % layout.Y;
                            yb = (y - 1 + layout.Y) % layout.Y;
                            zf = (z + 1 + layout.Z) % layout.Z;
                            zb = (z - 1 + layout.Z) % layout.Z;
                            tf = (t + 1 + layout.T) % layout.T;
                            tb = (t - 1 + layout.T) % layout.T;
                            spinorOut(x, y, z, t) += mul(U[0](x, y, z, t), spinorIn(xf, y, z, t)) - mul(Udag[0](x, y, z, t), spinorIn(xb, y, z, t));
                            spinorOut(x, y, z, t) += mul(U[1](x, y, z, t), spinorIn(x, yf, z, t)) - mul(Udag[1](x, y, z, t), spinorIn(x, yb, z, t));
                            spinorOut(x, y, z, t) += mul(U[2](x, y, z, t), spinorIn(x, y, zf, t)) - mul(Udag[2](x, y, z, t), spinorIn(x, y, zb, t));
                            spinorOut(x, y, z, t) += mul(U[3](x, y, z, t), spinorIn(x, y, z, tf)) - mul(Udag[3](x, y, z, t), spinorIn(x, y, z, tb));
                        }
                    }
                }
            }
        }
    }


    template <typename T>
    static void call(SpinorField<T> &spinorOut, const GaugeFieldNew<T> &U4, const SpinorField<T> &spinorIn)
    {
        int xf = 0, xb = 0;
        int yf = 0, yb = 0;
        int zf = 0, zb = 0;
        int tf = 0, tb = 0;
#pragma omp parallel for private(xf, xb, yf, yb, zf, zb, tf, tb)
        for (int t = 0; t < layout.T; t++) {
            for (int z = 0; z < layout.Z; z++) {
                for (int y = 0; y < layout.Y; y++) {
                    for (int x = 0; x < layout.X; x++) {
                        xf = (x + 1 + layout.X) % layout.X;
                        xb = (x - 1 + layout.X) % layout.X;
                        yf = (y + 1 + layout.Y) % layout.Y;
                        yb = (y - 1 + layout.Y) % layout.Y;
                        zf = (z + 1 + layout.Z) % layout.Z;
                        zb = (z - 1 + layout.Z) % layout.Z;
                        tf = (t + 1 + layout.T) % layout.T;
                        tb = (t - 1 + layout.T) % layout.T;
                        spinorOut(x, y, z, t) += mul(U4(x, y, z, t)[0], spinorIn(xf, y, z, t)) - mdagv(U4(xb, y, z, t)[0], spinorIn(xb, y, z, t));
                        spinorOut(x, y, z, t) += mul(U4(x, y, z, t)[1], spinorIn(x, yf, z, t)) - mdagv(U4(x, yb, z, t)[1], spinorIn(x, yb, z, t));
                        spinorOut(x, y, z, t) += mul(U4(x, y, z, t)[2], spinorIn(x, y, zf, t)) - mdagv(U4(x, y, zb, t)[2], spinorIn(x, y, zb, t));
                        spinorOut(x, y, z, t) += mul(U4(x, y, z, t)[3], spinorIn(x, y, z, tf)) - mdagv(U4(x, y, z, tb)[3], spinorIn(x, y, z, tb));
                    }
                }
            }
        }
    }
};

#else

#define BLOCK_SIZE 16
#include "HisqDslashOpt_with8buffers.h"
#undef BLOCK_SIZE

#endif

// #undef Debug




template <EvenOdd_t Leo, EvenOdd_t Reo, typename T>
void HisqDslashOpt(SpinorField<T> &spinorOut, const GaugeField<T> &U, const SpinorField<T> &spinorIn)
{
    HisqDslashOptImpl<Leo, Reo>::call(spinorOut, U, spinorIn);
}

template <EvenOdd_t Leo, EvenOdd_t Reo, typename T>
void HisqDslashOpt(SpinorField<T> &spinorOut, const GaugeField<T> &U, const GaugeField<T> &Udag, const SpinorField<T> &spinorIn)
{
    HisqDslashOptImpl<Leo, Reo>::call(spinorOut, U, Udag, spinorIn);
}


template <EvenOdd_t Leo, EvenOdd_t Reo, typename T>
void HisqDslashOpt(SpinorField<T> &spinorOut, const GaugeFieldNew<T> &U4, const SpinorField<T> &spinorIn)
{
    HisqDslashOptImpl<Leo, Reo>::call(spinorOut, U4, spinorIn);
}
