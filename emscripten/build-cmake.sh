#!/bin/sh
set -e

mkdir -p build
emcmake cmake --fresh -B build .

pushd build
emmake make -j$(nproc)
popd
