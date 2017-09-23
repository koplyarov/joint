[ "$COVERITY_BUILD" -ne 0 ] || exit 0

ARCHIVE="coverity_tool.tgz"

WithMsg "Testing token" test "${COVERITY_SCAN_TOKEN}" &&
WithMsg "Downloading coverity scan tool" wget --no-verbose https://scan.coverity.com/download/linux64 --post-data "token=${COVERITY_SCAN_TOKEN}&project=koplyarov%2Fjoint" -O "$ARCHIVE" &&
Verbose tar -xvzf "$ARCHIVE" ||
exit 1
