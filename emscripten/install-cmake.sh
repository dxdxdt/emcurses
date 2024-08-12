#!/bin/sh
set -e

pushd build
emmake make install
popd
