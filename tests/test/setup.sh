#!/bin/bash

files=$(find . -name "*.cpp" -and -not -wholename "./test/*")

cases=[]

echo "" > test/list_gen.h
echo '#include "tests.h"' > test/head_gen.h

for f in $files
do
	grep -o -E "TEST_CASE\([A-Za-z0-9_]*\)" $f | \
		sed 's/TEST_CASE/RUN_TEST_CASE/g' | \
		sed 's/$/;/' >> test/list_gen.h
	grep -o -E "TEST_CASE\([A-Za-z0-9_]*\)" $f | \
		sed 's/TEST_CASE/DEFINE_TEST_CASE/g' | \
		sed 's/$/;/' >> test/head_gen.h
done
