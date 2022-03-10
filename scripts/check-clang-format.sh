#!/bin/bash

# run clang format
make clang_format

git diff > clang_format.patch

if [ -s clang_format.patch ];
then
    echo "ERROR: Clang-format not run. See the diffs here:"
    # file not empty
    cat clang_format.patch
    exit 1
else
    # file empty
    echo "Code is formatted accordingly to the clang-format."
    rm clang_format.patch
    exit 0
fi

