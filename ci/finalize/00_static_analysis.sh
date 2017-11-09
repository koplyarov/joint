[ "$COVERITY_BUILD" -ne 0 ] || exit 0

ARCHIVE="joint.tgz"

WithMsg "Testing token" test "${COVERITY_SCAN_TOKEN}" &&
Verbose cd build &&
Verbose tar czvf "$ARCHIVE" cov-int &&
WithMsg "Uploading Coverity result" curl \
    --form token="${COVERITY_SCAN_TOKEN}" \
    --form email=koplyarov.da@gmail.com \
    --form file="@$ARCHIVE" \
    --form version="$(date --iso-8601)" \
    "https://scan.coverity.com/builds?project=koplyarov%2Fjoint" ||
exit 1
