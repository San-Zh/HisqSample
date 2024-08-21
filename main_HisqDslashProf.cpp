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


const int L = 32;
Layout    layout(L);

void test_HisqDslash(const int numLoop = 50)
{
    timer t;
    printf("Testing HisqDslash: %d  %d  %d  %d, Loop  %d\n", layout.X, layout.Y, layout.Z, layout.T, numLoop);

    SpinorField<float> spinorIn(layout);
    SpinorField<float> spinorOut(layout);
    SpinorField<float> spinorOut2(layout);
    SpinorField<float> spinorOut3(layout);
    spinorIn.random();
    // spinorOut.random();
    // spinorOut2.random();

    GaugeField<float> gaugeLink(layout);
    GaugeField<float> gaugeLinkDag(layout);
    gaugeLink.random();
    gaugeLinkDag.random();

    // GaugeFieldNew<float> gaugeLinkNew(layout);
    GaugeFieldNew<float> gaugeLinkNew(gaugeLink);
    // convertToGaugeFieldNew(gaugeLinkNew, gaugeLink);

    double memSizeSpinor = (double) (layout.vol() * 6 * sizeof(float)) / 1024.0;
    double memSizeGauge  = (double) (layout.vol() * 4 * 18 * sizeof(float)) / 1024.0;
    double numThreads    = (double) omp_get_max_threads();

    double memTotal = memSizeSpinor * 2.0 + memSizeGauge;

    printf("MemSize: SpinorField: %g KB, GaugeField: %g KB; Total: %g KB, Per Thread: %g KB\n", memSizeSpinor, memSizeGauge, memTotal, memTotal / numThreads);


    {
        double time      = 0.0;
        double flops     = (double) ((6 + 2) * 9 * 8 * numLoop) * (double) layout.volh() * 1.0e-9;
        double bandwidth = (double) (((3 + 3 + 9 + 9) * 4 + 3) * sizeof(std::complex<float>) * numLoop) * (double) layout.volh() * 1.0e-9;
        double intensity = flops / bandwidth;

        // Warm up
        for (int i = 0; i < 0; i++) {
            HisqDslash<ODD, EVEN>(spinorOut, gaugeLink, spinorIn);
            HisqDslash<EVEN, ODD>(spinorOut, gaugeLink, spinorIn);
            HisqDslash<FULL, FULL>(spinorOut2, gaugeLink, spinorIn);
        }

        // test HisqDslash<EVEN, ODD>, HisqDslash<ODD, EVEN>, and HisqDslash<FULL, FULL>
        t.reset();
        for (int i = 0; i < numLoop; i++) { HisqDslash<EVEN, ODD>(spinorOut, gaugeLink, spinorIn); }
        time = t.use_sec();
        printf("Dslash<E,O>  Performance: %8.3lf sec, %8.3lf GFLOPS/s, %8.3lf GB/s, %8.3lf flops/byte intensity\n", time, flops / time, bandwidth / time, intensity);

        t.reset();
        for (int i = 0; i < numLoop; i++) { HisqDslash<ODD, EVEN>(spinorOut, gaugeLink, spinorIn); }
        time = t.use_sec();
        printf("Dslash<O,E>  Performance: %8.3lf sec, %8.3lf GFLOPS/s, %8.3lf GB/s, %8.3lf flops/byte intensity\n", time, flops / time, bandwidth / time, intensity);

        t.reset();
        for (int i = 0; i < numLoop; i++) { HisqDslash<FULL, FULL>(spinorOut2, gaugeLink, spinorIn); }
        time = t.use_sec();
        printf("Dslash<F,F>  Performance: %8.3lf sec, %8.3lf GFLOPS/s, %8.3lf GB/s, %8.3lf flops/byte intensity\n", time, 2.0 * flops / time, 2.0 * bandwidth / time, intensity);
    }

#ifdef Debug
    double diff;
    for (int i = 0; i < layout.vol(); i++) { diff = norm2(spinorOut2(i) - spinorOut(i)); }
    printf("diff: %g\n", diff);
#endif

    // test HisqDslashOpt<FULL, FULL> and HisqDslashOptNew<FULL, FULL>
    {
        double time      = 0.0;
        double flops     = (double) ((6 + 2) * 9 * 8 * numLoop) * (double) layout.vol() * 1.0e-9;
        double bandwidth = (double) ((3 + (3 + 3 + 9 + 9) * 4) * sizeof(std::complex<float>) * numLoop) * (double) layout.vol() * 1.0e-9;
        double intensity = flops / bandwidth;

        t.reset();
        for (int i = 0; i < numLoop; i++) { HisqDslashOpt<FULL, FULL>(spinorOut, gaugeLink, spinorIn); }
        time = t.use_sec();
        printf("Dopt<F,F>    Performance: %8.3lf sec, %8.3lf GFLOPS/s, %8.3lf GB/s, %8.3lf flops/byte\n", time, flops / time, bandwidth / time, intensity);

        t.reset();
        for (int i = 0; i < numLoop; i++) { HisqDslashOpt<FULL, FULL>(spinorOut2, gaugeLinkNew, spinorIn); }
        time = t.use_sec();
        printf("Dopt2<F,F>   Performance: %8.3lf sec, %8.3lf GFLOPS/s, %8.3lf GB/s, %8.3lf flops/byte\n", time, flops / time, bandwidth / time, intensity);
#ifdef Debug
        for (int i = 0; i < layout.vol(); i++) { diff = norm2(spinorOut2(i) - spinorOut(i)); }
        printf("diff: %g\n", diff);
#endif

        t.reset();
        for (int i = 0; i < numLoop; i++) { HisqDslashOpt<FULL, FULL>(spinorOut3, gaugeLink, gaugeLinkDag, spinorIn); }
        time = t.use_sec();
        printf("DoptU^+<F,F> Performance: %8.3lf sec, %8.3lf GFLOPS/s, %8.3lf GB/s, %8.3lf flops/byte\n", time, flops / time, bandwidth / time, intensity);
    }
}

int main(int argc, char const *argv[])
{
    printf("\n----------------------------------------------------------------------------\n");
    layout.resize(L, L, L, L / 2);
    test_HisqDslash(10);

    printf("\n----------------------------------------------------------------------------\n");
    layout.resize(L, L, L, L);
    test_HisqDslash(10);

    printf("\n----------------------------------------------------------------------------\n");
    layout.resize(L, L, L, L * 2);
    test_HisqDslash(10);


    return 0;
}
