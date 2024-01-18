#!/bin/bash

if [[ -z "$ADVTEC9_DIR" ]]; then
    echo "Variable ADVTEC9_DIR empty, execution halted."
    exit 1
fi

set OS_VERSION="linux64"
set COMPILER_VERSION="gcc-11.2.1"
set BUILD_TYPE="release"

set ICU_VERSION="71.1"
set OPENSSL_VERSION="1.1.1t"
set ZLIB_VERSION="1.2.12"

export CMAKE_PREFIX_PATH="$ADVTEC9_DIR/third_libraries/icu/$ICU_VERSION/$OS_VERSION/include:$ADVTEC9_DIR/third_libraries/icu/$ICU_VERSION/$OS_VERSION/$COMPILER_VERSION/$BUILD_TYPE:$ADVTEC9_DIR/third_libraries/openssl/$OPENSSL_VERSION/$OS_VERSION/include:$ADVTEC9_DIR/third_libraries/openssl/$OPENSSL_VERSION/$OS_VERSION/$COMPILER_VERSION/$BUILD_TYPE:$ADVTEC9_DIR/third_libraries/zlib/$ZLIB_VERSION/$OS_VERSION/include:$ADVTEC9_DIR/third_libraries/zlib/$ZLIB_VERSION/$OS_VERSION/$COMPILER_VERSION/$BUILD_TYPE"

# OSR mode is enabled by default, add -DUSE_OSR=OFF to disable the OSR mode

BUILD_PROJECT=0

BUILD_DIR="$(pwd)/.build/linux.x86_64.release"

while getopts bi flag
do
    case "${flag}" in
        b) BUILD_PROJECT=1;;
    esac
done

echo ============== Config project ==============
cmake -G "Unix Makefiles" -S . -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release -DPROJECT_ARCH=x86_64 -DBUILD_DEMO=ON -DUSE_SANDBOX=ON -DCMAKE_INSTALL_PREFIX:PATH="$(pwd)/out/linux.x86_64.release"

if [ ${BUILD_PROJECT} -eq 1 ] 
then
    echo ============== Build project ==============
    cmake --build "${BUILD_DIR}"
fi
