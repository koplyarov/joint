sudo PYTHONPATH="`pwd`/build/bin:`pwd`/benchmarks/implementations/swig" ./build/bin/joint-benchmarks -c10 -v2 -t benchmarks/ci/template.py -o- > build/benchmarks/ci.py && 
python ./build/benchmarks/ci.py --data benchmarks/ci/canon_data.json ||
exit 1
