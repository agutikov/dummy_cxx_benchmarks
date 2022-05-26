#!/bin/sh


OUT_DIR=./output

mkdir -p ${OUT_DIR}/min_max


./build/clang/min_max_0 --benchmark_out=${OUT_DIR}/min_max/clang_O0.json
./build/clang/min_max_1 --benchmark_out=${OUT_DIR}/min_max/clang_O1.json
./build/clang/min_max_2 --benchmark_out=${OUT_DIR}/min_max/clang_O2.json
./build/clang/min_max_3 --benchmark_out=${OUT_DIR}/min_max/clang_O3.json

./build/gcc/min_max_0 --benchmark_out=${OUT_DIR}/min_max/gcc_O0.json
./build/gcc/min_max_1 --benchmark_out=${OUT_DIR}/min_max/gcc_O1.json
./build/gcc/min_max_2 --benchmark_out=${OUT_DIR}/min_max/gcc_O2.json
./build/gcc/min_max_3 --benchmark_out=${OUT_DIR}/min_max/gcc_O3.json







