# dummy_cxx_benchmarks
Some benchmarks of C++ implementations for simple tasks




```
git submodule update --init --recursive

CC=gcc CXX=g++ cmake -B ./build/gcc -S .
CC=clang CXX=clang++ cmake -B ./build/clang -S .

VERBOSE=1 make -C ./build/gcc -j
VERBOSE=1 make -C ./build/clang -j

make -C ./build/clang test

./run_benchmarks.sh
```





