#!/bin/sh

set -e

rm .config
cp config-base .config
./scripts/kconfig/merge_config.sh -m .config ../config-neptune

LLVM=1 make -j$(nproc)
