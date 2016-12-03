#!/bin/bash

# Bash script that download and build cpputest inside the project tree,
# where project's Makefile will look for it.

rm cpputest-3.8.tar.gz
wget https://github.com/cpputest/cpputest/releases/download/v3.8/cpputest-3.8.tar.gz
tar -xf cpputest-3.8.tar.gz
