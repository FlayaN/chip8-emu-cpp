#!/bin/sh

set -e
set -x

export CC=gcc-7
export CXX=g++-7
export PKG_CONFIG_PATH=$HOME/.local/lib/pkgconfig:$PKG_CONFIG_PATH

mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4