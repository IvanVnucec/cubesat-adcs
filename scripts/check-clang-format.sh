#!/bin/bash

# run clang format
make clang_format

git diff > clang_format.patch

# Delete if 0 size
if [ ! -s clang_format.patch ]
    cat clang_format.patch
    exit 1
then
    rm clang_format.patch
    exit 0
fi

