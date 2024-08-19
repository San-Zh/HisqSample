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
#include <vector>

#include "timer.h"
#include "HisqDslash.h"
#include "HisqDslashOpt.h"

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

    // std::vector<SU3F> GaugeLink(4);
    // for (int i = 0; i < 4; i++) { GaugeLink.emplace_back(latt); }
    SU3F **GaugeLink = new SU3F *[4];
    for (int i = 0; i < 4; i++) { GaugeLink[i] = new SU3F(layout); }
    for (int i = 0; i < 4; i++) { GaugeLink[i]->random(); }
    SU3F &U0 = *GaugeLink[0];
    SU3F &U1 = *GaugeLink[1];
    SU3F &U2 = *GaugeLink[2];
    SU3F &U3 = *GaugeLink[3];

    GaugeField<float> gaugeField(layout);
    gaugeField.random();

    // printf("Ready: random spinorIn data before HisqDslash\n");

    {
        t.reset();
        for (int i = 0; i < numLoop; i++) { HisqDslash<EVEN, ODD>(spinorOut, U0, spinorIn); }
        double time = t.use_sec();
        double flops = (double) ((6 + 2) * 9 * 2 * numLoop) * (double) layout.volh() * 1.0e-9 / time;
        double bandwidth = (double) ((3 + 3 + 9) * sizeof(std::complex<float>) * numLoop) * (double) layout.volh() * 1.0e-9 / time;
        double intensity = flops / bandwidth;
        printf("D_{e,o} Performance: %8.3lf sec, %8.3lf GFLOPS/s, %8.3lf GB/s, %8.3lf flops/byte intensity\n", time, flops, bandwidth, intensity);
    }

    // {
    //     t.reset();
    //     for (int i = 0; i < numLoop; i++) { HisqDslash<EVEN, ODD>(spinorOut, U0, spinorIn); }
    //     double time = t.use_sec();
    //     double flops = (double) ((6 + 2) * 9 * 2 * numLoop) * (double) layout.volh() * 1.0e-9 / time;
    //     double bandwidth = (double) ((3 + 3 + 9) * sizeof(std::complex<float>) * numLoop) * (double) layout.volh() * 1.0e-9 / time;
    //     double intensity = flops / bandwidth;
    //     printf("D_{o,e} Performance: %8.3lf sec, %8.3lf GFLOPS/s, %8.3lf GB/s, %8.3lf flops/byte intensity\n", time, flops, bandwidth, intensity);
    // }

    // {
    //     t.reset();
    //     for (int i = 0; i < numLoop; i++) { HisqDslash<FULL, FULL>(spinorOut, U0, spinorIn); }
    //     double time = t.use_sec();
    //     double flops = (double) ((6 + 2) * 9 * 2 * numLoop) * (double) layout.vol() * 1.0e-9 / time;
    //     double bandwidth = (double) ((3 + 3 + 9) * sizeof(std::complex<float>) * numLoop) * (double) layout.vol() * 1.0e-9 / time;
    //     double intensity = flops / bandwidth;
    //     printf("Dslash  Performance: %8.3lf sec, %8.3lf GFLOPS/s, %8.3lf GB/s, %8.3lf flops/byte intensity\n", time, flops, bandwidth, intensity);
    // }

    {
        t.reset();
        for (int i = 0; i < numLoop; i++) { HisqDslashOpt(spinorOut, GaugeLink, spinorIn); }
        double time = t.use_sec();
        double flops = (double) ((6 + 2) * 9 * 2 * 4 * numLoop) * (double) layout.vol() * 1.0e-9 / time;
        double bandwidth = (double) ((3 + (3 + 3) * 4 + 9*2*4 ) * sizeof(std::complex<float>) * numLoop) * (double) layout.vol() * 1.0e-9 / time;
        double intensity = flops / bandwidth;
        printf("Dslash  Performance: %8.3lf sec, %8.3lf GFLOPS/s, %8.3lf GB/s, %8.3lf flops/byte intensity\n", time, flops, bandwidth, intensity);
    }

    {
        t.reset();
        for (int i = 0; i < numLoop; i++) { HisqDslashOpt(spinorOut, gaugeField, spinorIn); }
        double time = t.use_sec();
        double flops = (double) ((6 + 2) * 9 * 2 * 4 * numLoop) * (double) layout.vol() * 1.0e-9 / time;
        double bandwidth = (double) ((3 + (3 + 3) * 4 + 9*2*4 ) * sizeof(std::complex<float>) * numLoop) * (double) layout.vol() * 1.0e-9 / time;
        double intensity = flops / bandwidth;
        printf("Dslash  Performance: %8.3lf sec, %8.3lf GFLOPS/s, %8.3lf GB/s, %8.3lf flops/byte intensity\n", time, flops, bandwidth, intensity);
    }
}

int main(int argc, char const *argv[])
{
    layout.resize(L, L, L, L);
    for (int i = 1; i < 4; i++) {
        printf("\n----------------------------------------------------------------------------\n");
        layout.resize(L, L, L, L * i);
        test_HisqDslash(10);
    }

    return 0;
}
  