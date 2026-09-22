#!/bin/sh

script="$1"
files="cqcc/*.cqcc"

echo "{"

for file in $files
do
    echo "    {"
    awk -f "$script" "$file" | sed 's/^/        /'
    echo "    },"
done

echo "};"
