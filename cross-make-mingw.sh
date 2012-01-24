#!/bin/sh

CMD_PREFIX="i586-mingw32msvc i686-w64-mingw32";

if [ "X$CC" = "X" ]; then
    for check in $CMD_PREFIX; do
        full_check="${check}-gcc"
	if [ ! $(which "$full_check") = "" ]; then
	    export CC="$full_check"
	fi
    done
fi

export PLATFORM=mingw32
export ARCH=x86

exec make $*
