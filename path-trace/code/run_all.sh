#!/usr/bin/env bash

# If project not ready, generate cmake file.
if [[ ! -d build ]]; then
    mkdir -p build
    cd build
    cmake ..
    cd ..
else
    rm -r build
    mkdir -p build
    cd build
    cmake ..
    cd ..
fi

# Build project.
cd build
make -j
cd ..

# Run all testcases. 
# You can comment some lines to disable the run of specific examples.
mkdir -p output

# bin/path-trace testcases/1.txt output/1.bmp
#time bin/path-trace testcases/2.txt output/2.bmp
time bin/path-trace testcases/4.txt output/4.bmp
time bin/path-trace testcases/uni.txt output/uni.bmp

#bin/path-trace testcases/3.txt output/3.bmp
