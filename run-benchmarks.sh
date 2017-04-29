#!/bin/bash

export PYTHONPATH="`pwd`/build/bin:`pwd`/benchmarks/swig"
clear
./build/bin/joint-benchmarks -c3 -v2 -t `pwd`/benchmarks/template.md -o-
