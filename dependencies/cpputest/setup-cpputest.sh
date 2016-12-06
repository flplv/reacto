#!/bin/bash

# Bash script that download and build cpputest inside the project tree,
# where project's Makefile will look for it.
if [ -z "$1" ]; then
    echo "Usage $0 <folder where cpputest will be installed>"
    exit -1
fi

if [ -e $1 ]; then
    if [ ! -d $1 ]; then
        echo "Path $1 exists and is not a folder."
        exit -1
    fi
fi

rm -rf $1

if [ ! -e './cpputest-3.8.tar.gz' ]; then
    wget https://github.com/cpputest/cpputest/releases/download/v3.8/cpputest-3.8.tar.gz
fi
tar -xf cpputest-3.8.tar.gz
mv cpputest-3.8 $1
