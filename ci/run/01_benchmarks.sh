[ "$RUN_BENCHMARKS" -ne 0 ] || exit 0

Verbose sudo PYTHONPATH="`pwd`/build/bin:`pwd`/benchmarks/implementations/swig" ./build/bin/joint-benchmarks -c10 -v3 -t benchmarks/ci/template.py -o- > build/benchmarks/ci.py && 
Verbose python ./build/benchmarks/ci.py --data benchmarks/ci/canon_data.json ||
exit 1
