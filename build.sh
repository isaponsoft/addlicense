#!/bin/sh
PRJDIR=$(dirname $0)
LIBAMTRS_SOURCE=ssh://hg@192.168.100.30:9122/isapon.repos/libs/libamtrs

if [ ! -d libamtrs ];
then
	echo "libamtrs download."
	git clone git@github.com:isaponsoft/libamtrs.git libamtrs
fi

cmake -DCMAKE_BUILD_TYPE=Release ${PRJDIR} && make
