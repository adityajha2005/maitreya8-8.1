#!/usr/bin/env bash

export PATH=/usr/bin:/bin

export CC=/usr/bin/gcc
export CXX=/usr/bin/g++

../configure  --disable-shared --with-libpng=builtin --with-libjpeg=builtin --with-libtiff=builtin

