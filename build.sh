#!/bin/sh
PRJDIR=$(dirname $0)

if [ ! -d libamtrs ];
then
	echo "libamtrs download."
	git clone git@github.com:isaponsoft/libamtrs.git libamtrs
fi

cmake -DCMAKE_BUILD_TYPE=Release ${PRJDIR} && make
