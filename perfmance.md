[TOC]

## CPU version
```log
Architecture:                       x86_64
CPU op-mode(s):                     32-bit, 64-bit
Byte Order:                         Little Endian
Address sizes:                      39 bits physical, 48 bits virtual
CPU(s):                             8
On-line CPU(s) list:                0-7
Thread(s) per core:                 2
Core(s) per socket:                 4
Socket(s):                          1
Vendor ID:                          GenuineIntel
CPU family:                         6
Model:                              142
Model name:                         Intel(R) Core(TM) i5-10210U CPU @ 1.60GHz
Stepping:                           12
CPU MHz:                            2112.006
BogoMIPS:                           4224.01
Hypervisor vendor:                  Microsoft
Virtualization type:                full
L1d cache:                          128 KiB
L1i cache:                          128 KiB
L2 cache:                           1 MiB
L3 cache:                           6 MiB
```

## Bandwidth
### STREAM
```shell
gcc -O3 -fopenmp -DSTREAM_ARRAY_SIZE=24000000 -DNTIMES=10 stream.c -o stream.24M
```

1. `export OMP_NUM_THREADS=4 ./stream.24M`
```
Number of Threads requested = 4
Number of Threads counted = 4
```
Function    |Best Rate MB/s | Avg time   |  Min time  |   Max time
------------|---------------|------------|------------|----------
Copy:       |    26386.2    | 0.014919   |  0.014553  |   0.015512
Scale:      |    17754.1    | 0.022379   |  0.021629  |   0.023076
Add:        |    19572.4    | 0.030612   |  0.029429  |   0.032997
Triad:      |    19359.4    | 0.030658   |  0.029753  |   0.033351

2. `export OMP_NUM_THREADS=8 ./stream.24M`
```
Number of Threads requested = 8
Number of Threads counted = 8
```
Function    |Best Rate MB/s | Avg time   |  Min time  |   Max time
------------|---------------|------------|------------|----------
Copy:       |    25158.7    | 0.016180   |  0.015263  |   0.018815
Scale:      |    17092.5    | 0.024122   |  0.022466  |   0.027076
Add:        |    18502.4    | 0.032625   |  0.031131  |   0.035763
Triad:      |    18464.5    | 0.033682   |  0.031195  |   0.037676


### Hisq
```shell
g++ -O3 -fopenmp -std=c++11 -Wall  -mavx2 -mfma -march=native -mtune=native  -o main_HisqDslashProf.bin  main_HisqDslashProf.cpp
```
1. `export OMP_NUM_THREADS=4`
```shell
export OMP_NUM_THREADS=4  && ./main_HisqDslashProf.bin # 2>&1 | tee log.output.txt
```
```
Testing HisqDslash: 32  32  32  32, Loop  50
D_{e,o} Performance:    0.774 sec,    4.878 GFLOPS/s,    4.065 GB/s,    1.200 flops/byte intensity
D_{o,e} Performance:    0.761 sec,    4.962 GFLOPS/s,    4.135 GB/s,    1.200 flops/byte intensity
Dslash  Performance:    1.469 sec,    5.139 GFLOPS/s,    4.282 GB/s,    1.200 flops/byte intensity
```
2. export OMP_NUM_THREADS=8
```shell
export OMP_NUM_THREADS=8  && ./main_HisqDslashProf.bin # 2>&1 | tee log.output.txt
```
```
Testing HisqDslash: 32  32  32  32, Loop  50
D_{e,o} Performance:    0.581 sec,    6.502 GFLOPS/s,    5.418 GB/s,    1.200 flops/byte intensity
D_{o,e} Performance:    0.627 sec,    6.019 GFLOPS/s,    5.016 GB/s,    1.200 flops/byte intensity
Dslash  Performance:    1.178 sec,    6.410 GFLOPS/s,    5.342 GB/s,    1.200 flops/byte intensity
```