#!/bin/sh

git submodule update --init --recursive

CC=gcc CXX=g++ cmake -B ./build/gcc -S .

VERBOSE=1 make -C ./build/gcc -j

CC=clang CXX=clang++ cmake -B ./build/clang -S .

VERBOSE=1 make -C ./build/clang -j

