#!/bin/sh

CMD_PREFIX="amd64-mingw32msvc x86_64-w64-mingw32";

if [ "X$CC" = "X" ]; then
    for check in $CMD_PREFIX; do
        full_check="${check}-gcc"
	if [ ! $(which "$full_check") = "" ]; then
	    export CC="$full_check"
	fi
    done
fi

export PLATFORM=mingw32
export ARCH=x64

exec make $*
