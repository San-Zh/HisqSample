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

#ifdef Debug
// #define GRID_BLOCK
#define BLOCK_SIZE 16
#endif

#ifndef Debug

template <typename T>
void HisqDslash(SpinorField<T> &spinorOut, const GaugeField<T> &U, const SpinorField<T> &spinorIn)
{
    int xf = 0, xb = 0;
    int yf = 0, yb = 0;
    int zf = 0, zb = 0;
    int tf = 0, tb = 0;
#pragma omp parallel for
    for (int t = 0; t < layout.T; t++) {
        for (int z = 0; z < layout.Z; z++) {
            for (int y = 0; y < layout.Y; y++) {
                for (int x = 0; x < layout.X; x += 1) {
                    xf = (x + 1 + layout.X) % layout.X;
                    xb = (x - 1 + layout.X) % layout.X;
                    yf = (y + 1 + layout.Y) % layout.Y;
                    yb = (y - 1 + layout.Y) % layout.Y;
                    zf = (z + 1 + layout.Z) % layout.Z;
                    zb = (z - 1 + layout.Z) % layout.Z;
                    tf = (t + 1 + layout.T) % layout.T;
                    tb = (t - 1 + layout.T) % layout.T;

                    spinorOut(x, y, z, t) += mul(U(x, y, z, t)[0], spinorIn(xf, y, z, t)) - mdagv(U(xb, y, z, t)[0], spinorIn(xb, y, z, t));
                    spinorOut(x, y, z, t) += mul(U(x, y, z, t)[1], spinorIn(x, yf, z, t)) - mdagv(U(x, yb, z, t)[1], spinorIn(x, yb, z, t));
                    spinorOut(x, y, z, t) += mul(U(x, y, z, t)[2], spinorIn(x, y, zf, t)) - mdagv(U(x, y, zb, t)[2], spinorIn(x, y, zb, t));
                    spinorOut(x, y, z, t) += mul(U(x, y, z, t)[3], spinorIn(x, y, z, tf)) - mdagv(U(x, y, z, tb)[3], spinorIn(x, y, z, tb));
                }
            }
        }
    }
}

template <typename T>
void HisqDslashOpt(SpinorField<T> &spinorOut, SU3Field<T> **&U, const SpinorField<T> &spinorIn)
{
    int xf = 0, xb = 0;
    int yf = 0, yb = 0;
    int zf = 0, zb = 0;
    int tf = 0, tb = 0;
#pragma omp parallel for
    for (int t = 0; t < layout.T; t++) {
        for (int z = 0; z < layout.Z; z++) {
            for (int y = 0; y < layout.Y; y++) {
#pragma unroll
                for (int x = 0; x < layout.X; x += 1) {
                    xf = (x + 1 + layout.X) % layout.X;
                    xb = (x - 1 + layout.X) % layout.X;
                    yf = (y + 1 + layout.Y) % layout.Y;
                    yb = (y - 1 + layout.Y) % layout.Y;
                    zf = (z + 1 + layout.Z) % layout.Z;
                    zb = (z - 1 + layout.Z) % layout.Z;
                    tf = (t + 1 + layout.T) % layout.T;
                    tb = (t - 1 + layout.T) % layout.T;

                    spinorOut(x, y, z, t) += mul((*U[0]) (x, y, z, t), spinorIn(xf, y, z, t)) - mdagv((*U[0]) (xb, y, z, t), spinorIn(xb, y, z, t));
                    spinorOut(x, y, z, t) += mul((*U[1]) (x, y, z, t), spinorIn(x, yf, z, t)) - mdagv((*U[1]) (x, yb, z, t), spinorIn(x, yb, z, t));
                    spinorOut(x, y, z, t) += mul((*U[2]) (x, y, z, t), spinorIn(x, y, zf, t)) - mdagv((*U[2]) (x, y, zb, t), spinorIn(x, y, zb, t));
                    spinorOut(x, y, z, t) += mul((*U[3]) (x, y, z, t), spinorIn(x, y, z, tf)) - mdagv((*U[3]) (x, y, z, tb), spinorIn(x, y, z, tb));
                }
            }
        }
    }
}

#else

// 调试版本，将x y 分割为若干个Block
#ifdef GRID_BLOCK
template <typename T>
void HisqDslash(SpinorField<T> &spinorOut, const SU3Field<T> &U0, const SpinorField<T> &spinorIn)
{
#pragma omp parallel for
    for (int t = 0; t < layout.T; t++) {
        for (int z = 0; z < layout.Z; z++) {
            // 将x y 分割为若干个Block
            const int Block_x = 16;
            const int Block_y = 16;
            for (int nBlock_y = 0; nBlock_y < layout.Y / Block_y; nBlock_y++) {
                for (int nBlock_x = 0; nBlock_x < layout.Y / Block_x; nBlock_x++) {
                    for (int iy = 0; iy < Block_y; iy++) {
                        int y = nBlock_y * Block_y + iy;
                        for (int ix = 0; ix < Block_x; ix += 1) {

                            int x = nBlock_x * Block_x + ix;
                            int xf = (x + 1 + layout.X) % layout.X;
                            int xb = (x - 1 + layout.X) % layout.X;
                            spinorOut(x, y, z, t) += mul(U0(x, y, z, t), spinorIn(xf, y, z, t)) - mdagv(U0(xb, y, z, t), spinorIn(xb, y, z, t));
                        }
                    }
                }
            }
        }
    }
}

#endif

#define TILE
// 调试版本，将x y 分割为若干个Tile, 并尝试使用shared buffer优化

#ifdef TILE

#endif

#endif

#undef Debug