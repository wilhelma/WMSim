#!/bin/bash

if ! [ -x "$(command -v clang-format)" ]; then
	echo 'Error: clang-format is not installed. aborting.' >&2
	exit 1
fi

dirs=("include" "src")

for dir in "${dirs[@]}"; do
	find ${dir} -regex '.*\.\(cpp\|h\)' -exec clang-format -i {} \; 
done

exit 0
