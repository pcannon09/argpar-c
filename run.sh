#!/bin/bash

# PCANNON RUN.SH v1.0S - FROM PCANNON PROJECT STANDARDS
# STANDARD: 20251212
# FROM STD: 20250608
# https://github.com/pcannon09/pcannonProjectStandards

PROJECT_INFO_PATH=".private/project.json"

projectName=$(jq -r '.exeName' $PROJECT_INFO_PATH)

if [[ "$1" == "GDB" ]]; then
	gdb ./build/${projectName}_exec

else
	./build/${projectName}_exec "$@"

fi

