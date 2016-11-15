#!/bin/bash

export PYTHONPATH="`pwd`/build/bin:`pwd`/benchmarks/python:`pwd`/benchmarks/swig"
clear
./build/bin/joint-benchmarks -v2 -t `pwd`/benchmarks/template.md -o-
