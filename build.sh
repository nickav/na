#!/bin/bash

set -e # exit on error

# Setup
script_path="$(cd "$(dirname "$0")" && pwd -P)"
project_root=$script_path

# Config
build_folder="$project_root/build"
bin_folder="$project_root/bin"
src_file="$project_root/src/main.cpp"
exe_name="nja"

flags="-std=c++11 -Wno-deprecated-declarations -Wno-int-to-void-pointer-cast -Wno-writable-strings -Wno-dangling-else -Wno-switch -Wno-undefined-internal"
libs="-framework Cocoa"

mkdir -p $build_folder

# Build executable
pushd $build_folder
  rm -rf cyan*
  time g++ $flags $libs -I$project_root/deps/ -D DEBUG $src_file -o $exe_name
  ./$exe_name
popd