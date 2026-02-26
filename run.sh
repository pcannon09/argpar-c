#!/bin/bash

# PCANNON RUN.SH v1.0S - FROM PCANNON PROJECT STANDARDS
# STANDARD: 20251212
# FROM STD: 20250608
# https://github.com/pcannon09/pcannonProjectStandards

PROJECT_INFO_PATH=".private/project.json"

projectName=$(jq -r '.exeName' $PROJECT_INFO_PATH)

readonly MAIN_FILE="build/${projectName}_exec"

if [[ "$1" == "GDB" ]]; then
	if [ -f "$MAIN_FILE" ]; then
		eval "gdb ./$MAIN_FILE $@"
	else
		eval "gdb ./$MAIN_FILE.exe $@"
	fi

else
	if [ -f "$MAIN_FILE" ]; then
		eval "./$MAIN_FILE $@"
	else
		eval "./$MAIN_FILE.exe $@"
	fi
fi

