/**
 * @file matrix.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <sys/time.h>
#include <stdio.h>
#include <iostream>
#include <complex>
#include <string.h>
#include <string>

#include "timer.h"
#include "HisqDslash.h"

// #define SINGLE_PREC

#ifndef SIZE
#define SIZE 4
#endif

// clang-format off
using SpinorF = SpinorField<float>;
using SU3F    = SU3Field<float>;
// clang-format on

const int L = 32;
Layout layout(L);

void test_HisqDslash(const int numLoop = 50)
{
    printf("Testing HisqDslash: %d  %d  %d  %d, Loop  %d\n", layout.X, layout.Y, layout.Z, layout.T, numLoop);

    timer t;
    SpinorF spinorIn(layout);
    SpinorF spinorOut(layout);
    spinorIn.random();
    spinorOut.random();

    SU3F U0(layout);
    U0.random();

    // printf("Ready: random spinorIn data before HisqDslash\n");

    t.reset();
    for (int i = 0; i < numLoop; i++) { HisqDslash(spinorOut, U0, spinorIn); }
    double time = t.use_sec();
    double flops = (double) ((6 + 2) * 9 * 2 * numLoop) * (double) layout.vol() * 1.0e-9 / time;
    double bandwidth = (double) ((3 + 3 + 9) * sizeof(std::complex<float>) * numLoop) * (double) layout.vol() * 1.0e-9 / time;
    double intensity = flops / bandwidth;
    printf("Performance: %8.3lf sec, %8.3lf GFLOPS/s, %8.3lf GB/s, %8.3lf flops/byte intensity\n", time, flops, bandwidth, intensity);
}

int main(int argc, char const *argv[])
{
    layout.resize(L, L, L, L);
    for (int i = 1; i < 8; i++) {
        printf("\n----------------------------------------------------------------------------\n");
        layout.resize(L, L, L, L * i);
        test_HisqDslash(50);
    }

    return 0;
}
