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

CheckLinuxPackageInRepo() {
    case `GetLinuxDistributorId` in
    "Ubuntu") apt-cache show "$1" 2>/dev/null | grep "^Version:" >/dev/null 2>/dev/null ;;
    *)
        echo "Unknown linux distribution!" >&2
        return 1
        ;;
    esac
}

CheckPymodule() {
    if ! which python >/dev/null 2>/dev/null; then
        echo "python interpreter not found!" >&2
        return 2
    fi
    python -c "import $1" >/dev/null 2>/dev/null
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
fi

MISSING_PYMODULES=""
for PYMODULE in $REQUIRED_PYMODULES; do
    if ! CheckPymodule "$PYMODULE"; then
        MISSING_PYMODULES="$MISSING_PYMODULES$PYMODULE "
    fi
done

if [ "$MISSING_PYMODULES" ]; then
    echo "Some python modules are missing, type the following to install them:"
    case `GetLinuxDistributorId` in
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
