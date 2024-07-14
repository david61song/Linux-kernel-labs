#!/bin/bash

echo "configure build output path" 


KERNEL_TOP_PATH="$(cd "$(dirname "$0")" ; pwd -P )"
KERN_VERSION="6.9.7"
OUTPUT="$KERNEL_TOP_PATH/out"
echo "$OUTPUT"

BUILD_LOG="$KERNEL_TOP_PATH/preprocess_build_log.txt"

PREPROCESS_FILE=$1
echo "build preprocessed file: $PREPROCESS_FILE"

echo "move kernel source"
cd linux-$KERN_VERSION


echo "make defconfig!"
make O=$OUTPUT defconfig 

echo "kernel build"
make $PREPROCESS_FILE O=$OUTPUT -j4 2>&1 | tee $BUILD_LOG
