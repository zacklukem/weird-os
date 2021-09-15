#!/bin/bash

files=$(find tests -name "*.c" -and -not -wholename "tests/test/*")

cases=[]

echo "" > tests/test/list_gen.h

for f in $files
do
	grep -o -E "TEST_CASE\([A-Za-z0-9_]*\)" $f | \
		sed 's/TEST_CASE/RUN_TEST_CASE/g' | \
		sed 's/$/;/' >> tests/test/list_gen.h
done
