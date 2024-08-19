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

// 使用标签分发（Tag Dispatching)
// 创建一个辅助结构或函数来根据参数选择不同的实现。编译器根据标签选择对应的模板函数

#pragma once

#include <omp.h>
#include "Field.h"

enum EvenOdd_t { EVEN, ODD, FULL };

// 模板结构体 HisqDslashImpl，用于实现不同类型的Dslash操作
template <EvenOdd_t Leo, EvenOdd_t Reo>
struct HisqDslashImpl {
    // 静态模板方法 call，用于执行Dslash操作，当前未实现
    template <typename T>
    static void call(SpinorField<T> &spinorOut, const SU3Field<T> &U0, const SpinorField<T> &spinorIn)
    {
        throw std::runtime_error("Error: NO Implemented! Only <EVEN, ODD>, <ODD, EVEN>, and <FULL, FULL> are supported.");
    }
};

// 特化模板结构体 HisqDslashImpl<EVEN, ODD>，用于实现EVEN到ODD类型的Dslash操作
template <>
struct HisqDslashImpl<EVEN, ODD> {
    template <typename T>
    static void call(SpinorField<T> &spinorOut, const SU3Field<T> &U0, const SpinorField<T> &spinorIn)
    {
#pragma omp parallel for collapse(2)
        for (int t = 0; t < layout.T; t++) {
            for (int z = 0; z < layout.Z; z++) {
                for (int y = 0; y < layout.Y; y++) {
                    switch ((t + z + y) % 2) {
                    case EVEN:
                        for (int x = 0; x < layout.X; x += 2) {
                            int xf = (x + 1 + layout.X) % layout.X;
                            int xb = (x - 1 + layout.X) % layout.X;
                            spinorOut(x, y, z, t) += mul(U0(x, y, z, t), spinorIn(xf, y, z, t)) - mdagv(U0(xb, y, z, t), spinorIn(xb, y, z, t));
                        }
                        break;
                    case ODD:
                        for (int x = 1; x < layout.X; x += 2) {
                            int xf = (x + 1 + layout.X) % layout.X;
                            int xb = (x - 1 + layout.X) % layout.X;
                            spinorOut(x, y, z, t) += mul(U0(x, y, z, t), spinorIn(xf, y, z, t)) - mdagv(U0(xb, y, z, t), spinorIn(xb, y, z, t));
                        }
                        break;
                    }
                }
            }
        }
    }
};

// 特化模板结构体 HisqDslashImpl<ODD, EVEN>，用于实现ODD到EVEN类型的Dslash操作
template <>
struct HisqDslashImpl<ODD, EVEN> {
    template <typename T>
    static void call(SpinorField<T> &spinorOut, const SU3Field<T> &U0, const SpinorField<T> &spinorIn)
    {
#pragma omp parallel for collapse(2)
        for (int t = 0; t < layout.T; t++) {
            for (int z = 0; z < layout.Z; z++) {
                for (int y = 0; y < layout.Y; y++) {
                    switch ((t + z + y) % 2) {
                    case EVEN:
                        for (int x = 1; x < layout.X; x += 2) {
                            int xf = (x + 1 + layout.X) % layout.X;
                            int xb = (x - 1 + layout.X) % layout.X;
                            spinorOut(x, y, z, t) += mul(U0(x, y, z, t), spinorIn(xf, y, z, t)) - mdagv(U0(xb, y, z, t), spinorIn(xb, y, z, t));
                        }
                        break;
                    case ODD:
                        for (int x = 0; x < layout.X; x += 2) {
                            int xf = (x + 1 + layout.X) % layout.X;
                            int xb = (x - 1 + layout.X) % layout.X;
                            spinorOut(x, y, z, t) += mul(U0(x, y, z, t), spinorIn(xf, y, z, t)) - mdagv(U0(xb, y, z, t), spinorIn(xb, y, z, t));
                        }
                        break;
                    }
                }
            }
        }
    }
};

// 特化模板结构体 HisqDslashImpl<FULL, FULL>，用于实现FULL类型的Dslash操作
// 静态模板方法 call，用于执行FULL类型的Dslash操作
#ifndef Debug
template <>
struct HisqDslashImpl<FULL, FULL> {
    template <typename T>
    static void call(SpinorField<T> &spinorOut, const SU3Field<T> &U0, const SpinorField<T> &spinorIn)
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
};
#else
// 特化模板结构体 HisqDslashImpl<FULL, FULL>，用于实现FULL类型的Dslash操作
template <>
struct HisqDslashImpl<FULL, FULL> {
    template <typename T>
    static void call(SpinorField<T> &spinorOut, const SU3Field<T> &U0, const SpinorField<T> &spinorIn)
    {
        HisqDslashImpl<EVEN, ODD>::call(spinorOut, U0, spinorIn);
        HisqDslashImpl<ODD, EVEN>::call(spinorOut, U0, spinorIn);
    }
};
#endif

// 模板函数 HisqDslash
// 作用：执行HisqDslash操作，根据标签选择不同的实现。
// 表达式： $ spinorOut_{Le} = D(U0)_{Leo,Reo} * spinorIn_{Reo} $
// 参数：
//   spinorOut - 输出自旋场
//   U0 - SU3场
//   spinorIn - 输入自旋场
// 模板参数：
//   Leo - 偶奇类型
//   Reo - 偶奇类型
//   T - 数据类型
template <EvenOdd_t Leo, EvenOdd_t Reo, typename T>
void HisqDslash(SpinorField<T> &spinorOut, const SU3Field<T> &U0, const SpinorField<T> &spinorIn)
{
    HisqDslashImpl<Leo, Reo>::call(spinorOut, U0, spinorIn);
}
