[ "$TEST_COVERAGE" -eq 0 ] && exit 0

echo "Sending coverage info"

lcov --directory . --capture --output-file coverage.info &&
lcov --remove coverage.info '/usr/*' 'test/*' 'benchmarks/*' 'examples/*' 'build/*' 'bindings/java/*' --output-file coverage.info &&
lcov --list coverage.info && coveralls-lcov --repo-token ${COVERALLS_TOKEN} coverage.info ||
exit 1
