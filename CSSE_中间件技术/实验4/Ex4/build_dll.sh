#!/bin/bash
# Build JniDemo.dll using MinGW g++
# Run this from the Ex4 project root.

set -e

JDK_HOME="E:/Programs/JDK"
INCLUDE="-I$JDK_HOME/include -I$JDK_HOME/include/win32"
CPP_FILE="cpp/Demo.cpp"
OUTPUT="lib/JniDemo.dll"

echo "=== Compiling JniDemo.dll ==="
g++ -shared -o "$OUTPUT" "$CPP_FILE" $INCLUDE -std=c++17 -O2 -static-libgcc -static-libstdc++
echo "=== Build successful: $OUTPUT ==="
