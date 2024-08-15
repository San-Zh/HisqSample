#!Makefile

CXX      := g++
CXX_FLAGS:= -O3 -fopenmp -std=c++11 -Wall 
CXX_FLAGS+= -mavx2 -mfma -march=native -mtune=native -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-function -Wno-unused-local-typedefs -Wno-maybe-uninitialized -Wno-strict-aliasing -Wno-sign-compare -Wno-reorder -Wno-missing-field-initializers -Wno-comment -Wno-switch-enum -Wno-unknown-pragmas -Wno-unused-private-field -Wno-unused-label -Wno-unused-value -Wno-parentheses -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-local-typedefs -Wno-maybe-uninitialized -Wno-strict

# CXX      := clang++ 

SOURCE = ./
VPATH = $(SOURCE)

ALL: main_HisqDslashProf.bin

TARGET:

%.o: ${SOURCE}/%.c
	${CC} ${C_FLAGS} -o $@ -c $<

%.o: ${SOURCE}/%.cpp
	${CXX} ${CXXF_LAGS} -o $@ -c $<

main_HisqDslashProf.bin: main_HisqDslashProf.cpp
	${CXX} ${CXX_FLAGS} -o $@  $^

clean:
	rm *.o main_HisqDslashProf.bin -rf

run: main_HisqDslashProf.bin
	export OMP_NUM_THREADS=16  && ./main_HisqDslashProf.bin # 2>&1 | tee log.output.txt

.PHONY: clean run