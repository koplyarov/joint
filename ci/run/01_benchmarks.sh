[ "$RUN_BENCHMARKS" -ne 0 ] || exit 0

[ "$BENCHMARKS_NUM_PASSES" ] || BENCHMARKS_NUM_PASSES=2

Verbose "$SOURCE_DIR/benchmarks/core/bin/benchmarks-ci.py" \
    --executable "$MAIN_DIR/build/bin/joint-benchmarks" \
    --reference-executable "$REFERENCE_JOINT_DIR/build/bin/joint-benchmarks" \
    --env-update "{\"PYTHONPATH\": \"$MAIN_DIR/build/bin/bin\"}" \
    --reference-env-update "{\"PYTHONPATH\": \"$REFERENCE_JOINT_DIR/build/bin/bin\"}" \
    --benchmarks "$SOURCE_DIR/benchmarks/ci/benchmarks.json" \
    --num-passes "$BENCHMARKS_NUM_PASSES" ||
exit 1
