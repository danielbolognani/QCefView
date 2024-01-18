#!/bin/bash

if [[ -z "$ADVTEC9_DIR" ]]; then
    echo "Variable ADVTEC9_DIR empty, execution halted."
    exit 1
fi

set OS_VERSION="mac64"
set COMPILER_VERSION=""
set BUILD_TYPE="release"

set ICU_VERSION="71.1"
set OPENSSL_VERSION="1.1.1t"
set ZLIB_VERSION="1.2.12"

export CMAKE_PREFIX_PATH="$ADVTEC9_DIR/third_libraries/icu/$ICU_VERSION/$OS_VERSION/include:$ADVTEC9_DIR/third_libraries/icu/$ICU_VERSION/$OS_VERSION/$BUILD_TYPE:$ADVTEC9_DIR/third_libraries/openssl/$OPENSSL_VERSION/$OS_VERSION/include:$ADVTEC9_DIR/third_libraries/openssl/$OPENSSL_VERSION/$OS_VERSION/$BUILD_TYPE:$ADVTEC9_DIR/third_libraries/zlib/$ZLIB_VERSION/$OS_VERSION/include:$ADVTEC9_DIR/third_libraries/zlib/$ZLIB_VERSION/$OS_VERSION/$BUILD_TYPE"

# OSR mode is enabled by default, add -DUSE_OSR=OFF to disable the OSR mode

BUILD_PROJECT=0

BUILD_DIR="$(pwd)/.build/macos.x86_64.release"

while getopts bi flag
do
    case "${flag}" in
        b) BUILD_PROJECT=1;;
    esac
done

echo ============== Config project ==============
cmake -G "Xcode" -S . -B "${BUILD_DIR}" -DPROJECT_ARCH=x86_64 -DBUILD_DEMO=ON -DUSE_SANDBOX=ON -DCMAKE_INSTALL_PREFIX:PATH="$(pwd)/out/macos.x86_64" -DOPENSSL_INCLUDE_DIR=/Users/bamboo/Qt/6.2.5/depends/include -DCMAKE_BUILD_TYPE=Release -DICU_ROOT=/Users/bamboo/Qt/6.2.5/depends -DICU_INCLUDE_DIRS=/Users/bamboo/Qt/6.2.5/depends/include

if [ ${BUILD_PROJECT} -eq 1 ] 
then
    echo ============== Build project ==============
    cmake --build "${BUILD_DIR}" --config Release
fi
