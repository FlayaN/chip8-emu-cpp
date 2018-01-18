#!/bin/sh

set -e
set -x

mkdir -p $HOME/.local
curl -L https://cmake.org/files/v3.8/cmake-3.8.0-Linux-x86_64.tar.gz \
    | tar -xz -C $HOME/.local --strip-components=1