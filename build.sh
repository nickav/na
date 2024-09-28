#!/bin/bash
set -e # exit on error

# Config
script_path="$(cd "$(dirname "$0")" && pwd -P)"
project_root=$script_path
exe_name="na"
flags="-Wno-deprecated-declarations -Wno-int-to-void-pointer-cast -Wno-writable-strings -Wno-dangling-else -Wno-switch -Wno-undefined-internal"
libs="-framework Cocoa"

pushd $project_root

    mkdir -p build

    # Build executable
    pushd build
        rm -rf cyan*

        time clang++ -std=c++11 $flags $libs -I$project_root/deps/ -D DEBUG=1 ../test/main.cpp -o $exe_name
        ./$exe_name

        #time clang $flags $libs -I$project_root/deps/ -D DEBUG=1 ../test/main.c -o $exe_name
        #./$exe_name

        time clang++ -std=c++11 $flags $libs -I$project_root/deps/ -D DEBUG=1 ../test/na_inliner.cpp -o na_inliner
    popd

popd