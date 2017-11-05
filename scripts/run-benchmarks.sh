#!/bin/bash

SCRIPT_DIR="$(dirname $(readlink -f $0))"
ROOT_DIR="$SCRIPT_DIR/.."

if [ $# -ne 1 ]; then
	echo "Usage: $0 <template file>" >&2
	exit 1
fi

TEMPLATE="$1"

export PYTHONPATH="$ROOT_DIR/build/bin:$ROOT_DIR/benchmarks/implementations/swig"
clear
"$ROOT_DIR/build/bin/joint-benchmarks" -c3 -v2 -t "$ROOT_DIR/$TEMPLATE" -o-
