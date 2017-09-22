[ "$TEST_COVERAGE" -eq 0 ] && exit 0

Verbose() {
	$@
	if [ $? -ne 0 ]; then
		echo "$* failed"
		return 1
	fi
}

InstallJinja2Coverage() {
	Verbose mkdir thirdparty &&
	Verbose cd thirdparty &&
	Verbose git clone https://github.com/MrSenko/coverage-jinja-plugin.git &&
	(
		cd coverage-jinja-plugin &&
		Verbose git checkout e074018d3e3854bf39a588ef62abc4936c43c2a4 &&
		Verbose sudo python setup.py install
	)
}

Verbose InstallJinja2Coverage &&
Verbose echo "[run]
plugins =
    jinja_coverage

[jinja_coverage]
template_directory = $(pwd)/joint-gen/joint/templates" > .coveragerc ||
exit 1
