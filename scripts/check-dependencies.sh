#!/bin/bash

LINUX_DISTRIB_ID=""
GetLinuxDistributorId() {
	if [ -z "$SHSTUFF_LINUX_DISTRIB_ID" ]; then
		if uname | grep -qi "mingw"; then
			LINUX_DISTRIB_ID="mingw"
		else
			which lsb_release >/dev/null 2>/dev/null || { Log Error "lsb_release utility not found!"; return 1; }
			LINUX_DISTRIB_ID=`lsb_release -is`
		fi
	fi
	echo "$LINUX_DISTRIB_ID"
}

CheckLinuxPackage() {
	case `GetLinuxDistributorId` in
	"Ubuntu") dpkg -s $1 >/dev/null 2>/dev/null ;;
	*)
		echo "Unknown linux distribution!" >&2
		return 1
		;;
	esac
}

case `GetLinuxDistributorId` in
"Ubuntu")
	REQUIRED_PACKAGES="
		bison
		build-essential
		cmake
		flex
		g++
		git
		libboost-all-dev
		openjdk-8-jdk;openjdk-9-jdk
		python-colorama
		python-coverage
		python-dev
		python-jinja2
		python-pip
		python-pyparsing
		swig
		valgrind
	"
	;;
*)
	echo "Unknown linux distribution!" >&2
	exit 1
	;;
esac

MISSING_PACKAGES=""
for PACKAGES_LIST in $REQUIRED_PACKAGES; do
	NEW_MISSING_PACKAGE=""
	OLD_IFS="$IFS"; IFS=";"
	for PACKAGE in $PACKAGES_LIST; do
		if CheckLinuxPackage "$PACKAGE"; then
			NEW_MISSING_PACKAGE=""
			break
		else
			NEW_MISSING_PACKAGE="$PACKAGE"
		fi
	done
	IFS="$OLD_IFS"
	[ "$NEW_MISSING_PACKAGE" ] && MISSING_PACKAGES="$MISSING_PACKAGES$NEW_MISSING_PACKAGE "
done

if [ "$MISSING_PACKAGES" ]; then
	echo "Some packages are missing, type the following to install them:"
	case `GetLinuxDistributorId` in
	"Ubuntu") echo "sudo apt-get install $MISSING_PACKAGES" ;;
	*)
		echo "Unknown linux distribution, here are the packages"
		echo "$MISSING_PACKAGES"
		;;
	esac
else
	echo "You have all the dependencies installed"
fi
