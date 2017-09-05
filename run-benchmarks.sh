#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: $0 <template file>" >&2
	exit 1
fi

TEMPLATE="$1"

export PYTHONPATH="`pwd`/build/bin:`pwd`/benchmarks/implementations/swig"
clear
./build/bin/joint-benchmarks -c3 -v2 -t "`pwd`/$TEMPLATE" -o-
