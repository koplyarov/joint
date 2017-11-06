export NUM_CORES=$(grep -c ^processor /proc/cpuinfo)

NormalizePath() {
	echo "$(readlink -m "$1")"
}

NormalizedDirName() {
	echo "$(dirname "$(NormalizePath "$1")")"
}

StringContains() {
	[ -z "${1##*$2*}" ]
}

Log() {
	echo "$*" >&2
}

Fail() {
	Log "$@"
	exit 1
}

WithMsg() {
	local MSG
	MSG="$1"
	shift

	Log "$MSG"
	"$@"
	if [ $? -ne 0 ]; then
		Log "ERROR: $MSG failed"
		return 1
	fi
}

Verbose() {
	WithMsg "$*" "$@"
}

ZeroInit() {
	while [ $# -gt 0 ]; do
		eval "[ \"\$$1\" ] || export $1=0"
		shift
	done
}
