[ "$TEST_COVERAGE" -eq 0 ] && exit 0

COVERAGERC_PATH="$(pwd)/.coveragerc"

Verbose() {
	echo "$*" >&2
	$@
	if [ $? -ne 0 ]; then
		echo "ERROR: $* failed" >&2
		return 1
	fi
}

InstallJinja2Coverage() {
	(
		Verbose mkdir -p thirdparty &&
		Verbose cd thirdparty &&
		Verbose git clone https://github.com/MrSenko/coverage-jinja-plugin.git &&
		Verbose cd coverage-jinja-plugin &&
		Verbose git checkout e074018d3e3854bf39a588ef62abc4936c43c2a4 &&
		Verbose sudo python setup.py install
	)
}

MakeCoverageRc() {
	echo "Creating $1" &&
	echo "[run]
plugins =
	jinja_coverage

[jinja_coverage]
template_directory = $(pwd)/joint-gen/joint/templates" > "$1"
}

InstallJinja2Coverage &&
MakeCoverageRc "$COVERAGERC_PATH" ||
exit 1
