#!/bin/bash

# OSR mode is enabled by default, add -DUSE_OSR=OFF to disable the OSR mode

BUILD_PROJECT=0

BUILD_DIR="$(pwd)/.build/linux.x86_64.debug"

while getopts bi flag
do
    case "${flag}" in
        b) BUILD_PROJECT=1;;
    esac
done

echo ============== Config project ==============
cmake -G "Unix Makefiles" -S . -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Debug -DPROJECT_ARCH=x86_64 -DBUILD_DEMO=ON -DUSE_SANDBOX=ON -DCMAKE_INSTALL_PREFIX:PATH="$(pwd)/out/linux.x86_64.debug"

if [ ${BUILD_PROJECT} -eq 1 ] 
then
    echo ============== Build project ==============
    cmake --build "${BUILD_DIR}"
fi
