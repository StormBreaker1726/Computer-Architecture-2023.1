#!/bin/bash

dir_path="../build"

if [ ! -d "$dir_path" ]; then
    # If the directory doesn't exist, create it
    mkdir -p "$dir_path"
fi

cd $dir_path

cmake ../.

make clean all

./taca


