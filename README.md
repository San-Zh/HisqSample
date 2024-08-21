# HISQ Dslash Sample Code
[toc]

This directory contains the HISQ Dslash code for the Lattice QCD simulations.
- OpenMP is supported. 
- MPI is not supported.

## Directory Structure

```
├── README.md
├── Makefile                # Contains the makefile for compilation
├── Field.h                 # Contains the field class
├── HisqDslash.h            # Contains the HisqDslash class and functions
├── HisqDslashOpt.h         # Contains the Optimized HisqDslashOpt
├── main_HisqDslashProf.cpp # Contains the main function for profiling
├── Layout.cpp
├── Layout.h
├── mat33.h
├── timer.h
├── vec3.h
└── vec4.h
```

## Compilation

To compile the code, simply run `make` in the directory. This will create an executable file named `main_HisqDslashProf.bin`.

## Running the Code
1. To run the code, simply execute the `main_HisqDslashProf.bin` executable file.  
   Or, you can run the code using `make run`.
2. To use `N` OpenMP threads, you can set the environment variable with `export OMP_NUM_THREADS=N`.
   
For example, to run the code with 4 OpenMP threads, you can run the following commands:
``` shell
export OMP_NUM_THREADS=4 && ./main_HisqDslashProf.bin
```