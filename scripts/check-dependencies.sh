#!/bin/bash

REQUIRED_PYMODULES="
    pylint_quotes
"

REQUIRED_PACKAGES="
    bison
    build-essential
    cmake
    flex
    g++
    git
    mypy
    openjdk-7-jdk;openjdk-8-jdk;openjdk-9-jdk
    pep8
    pylint
    python-colorama
    python-coverage
    python-dev
    python-jinja2
    python-pip
    python-pyparsing
    swig
    valgrind
"

################################################################################

if [ -z "$LINUX_DISTRIB_ID" ]; then
    if uname | grep -qi "mingw"; then
        LINUX_DISTRIB_ID="mingw"
    else
        which lsb_release >/dev/null 2>/dev/null || { Log Error "lsb_release utility not found!"; return 1; }
        LINUX_DISTRIB_ID=`lsb_release -is`
    fi
fi
if [ -z "$LINUX_DISTRIB_ID" ]; then
    echo "Unknown linux distribution!" >&2
    exit 1
fi

CheckLinuxPackage() {
    local PACKAGE=$1

    case "$LINUX_DISTRIB_ID" in
    "Ubuntu") dpkg -s $PACKAGE >/dev/null 2>/dev/null ;;
    *) echo "Unknown linux distribution!" >&2; return 1 ;;
    esac
}

CheckLinuxPackageInRepo() {
    local PACKAGE=$1

    case "$LINUX_DISTRIB_ID" in
    "Ubuntu") apt-cache show "$PACKAGE" 2>/dev/null | grep "^Version:" >/dev/null 2>/dev/null ;;
    *) echo "Unknown linux distribution!" >&2; return 1 ;;
    esac
}

FindFileInPackage() {
    local PACKAGE=$1
    local FILE=$2

    case "$LINUX_DISTRIB_ID" in
    "Ubuntu") apt-file search "$FILE" 2>/dev/null | grep -F "$PACKAGE" 2>/dev/null | head -n1 | sed -n "s/^[^:]\+: //p" ;;
    *) echo "Unknown linux distribution!" >&2; return 1 ;;
    esac
}

CheckPymodule() {
    if ! which python >/dev/null 2>/dev/null; then
        echo "python interpreter not found!" >&2
        return 2
    fi
    python -c "import $1" >/dev/null 2>/dev/null
}

CheckEnvVariable() {
    local VAR=$1
    local CANDIDATE=$2

    if eval "test -z \"\$$VAR\""; then
        echo "You should set the $VAR environment variable"
        if [ "$CANDIDATE" ]; then
            echo "export $VAR=\"$CANDIDATE\""
        fi
    fi
}

SafeDirName() {
    local SRC_PATH=$1
    local COUNT=$2
    local RESULT=$1
    local I

    [ "$COUNT" ] || COUNT=1

    for ((I=0; I<$COUNT; ++I)); do
        [ -z "$RESULT" ] && return
        [ "$RESULT" == "$(basename "$RESULT")" ] && return
        RESULT=$(dirname "$RESULT")
    done

    echo "$RESULT"
}

PackageHook() {
    local PACKAGE=$1

    case "$PACKAGE" in
    *jdk*) JAVA_HOME_CANDIDATE=$(SafeDirName $(FindFileInPackage $PACKAGE jni.h) 2) ;;
    esac
}

################################################################################

MISSING_PACKAGES=""
for PACKAGES_LIST in $REQUIRED_PACKAGES; do
    NEW_MISSING_PACKAGE=""
    OLD_IFS="$IFS"; IFS=";"
    PACKAGES_LIST_IS_OK=0
    for PACKAGE in $PACKAGES_LIST; do
        CheckLinuxPackageInRepo "$PACKAGE" || continue
        PACKAGES_LIST_IS_OK=1
        if CheckLinuxPackage "$PACKAGE"; then
            NEW_MISSING_PACKAGE=""
            PackageHook "$PACKAGE"
            break
        else
            NEW_MISSING_PACKAGE="$PACKAGE"
        fi
    done
    IFS="$OLD_IFS"
    if [ "$PACKAGES_LIST_IS_OK" -eq 0 ]; then
        echo "WARNING: No packages from this list available in your system repo:"
        echo "$PACKAGES_LIST"
    fi
    if [ "$NEW_MISSING_PACKAGE" ]; then
        MISSING_PACKAGES="$MISSING_PACKAGES$NEW_MISSING_PACKAGE "
        PackageHook "$NEW_MISSING_PACKAGE"
    fi
done

if [ "$MISSING_PACKAGES" ]; then
    echo "Some packages are missing, type the following to install them:"
    case "$LINUX_DISTRIB_ID" in
    "Ubuntu") echo "sudo apt-get install $MISSING_PACKAGES" ;;
    *)
        echo "Unknown linux distribution, here are the packages"
        echo "$MISSING_PACKAGES"
        ;;
    esac
fi

MISSING_PYMODULES=""
for PYMODULE in $REQUIRED_PYMODULES; do
    if ! CheckPymodule "$PYMODULE"; then
        MISSING_PYMODULES="$MISSING_PYMODULES$PYMODULE "
    fi
done

if [ "$MISSING_PYMODULES" ]; then
    echo "Some python modules are missing, type the following to install them:"
    case "$LINUX_DISTRIB_ID" in
    "Ubuntu") echo "sudo pip install $MISSING_PYMODULES" ;;
    *)
        echo "Unknown linux distribution, here are the modules"
        echo "$MISSING_PYMODULES"
        ;;
    esac
fi

if [ -z "$MISSING_PACKAGES" -a -z "$MISSING_PYMODULES" ]; then
    echo "You have all the dependencies installed"
fi

CheckEnvVariable JAVA_HOME $JAVA_HOME_CANDIDATE
