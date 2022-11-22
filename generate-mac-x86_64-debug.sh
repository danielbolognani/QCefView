#!/bin/bash

# OSR mode is enabled by default, add -DUSE_OSR=OFF to disable the OSR mode

BUILD_PROJECT=0

BUILD_DIR="$(pwd)/.build/macos.x86_64.debug"

while getopts bi flag
do
    case "${flag}" in
        b) BUILD_PROJECT=1;;
    esac
done

echo ============== Config project ==============
cmake -G "Xcode" -S . -B "${BUILD_DIR}" -DPROJECT_ARCH=x86_64 -DCMAKE_OSX_ARCHITECTURES=x86_64 -DBUILD_DEMO=ON -DUSE_SANDBOX=OFF -DCMAKE_INSTALL_PREFIX:PATH="$(pwd)/out/macos.x86_64" -DOPENSSL_INCLUDE_DIR=/Users/bamboo/Qt/6.2.5/depends/include -DCMAKE_BUILD_TYPE=DEBUG -DICU_ROOT=/Users/bamboo/Qt/6.2.5/depends -DICU_INCLUDE_DIRS=/Users/bamboo/Qt/6.2.5/depends/include

if [ ${BUILD_PROJECT} -eq 1 ] 
then
    echo ============== Build project ==============
    cmake --build "${BUILD_DIR}" --config Debug
fi
