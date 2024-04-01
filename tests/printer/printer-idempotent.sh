#!/bin/sh

SCRIPT_DIR=$(dirname "$0")
COMPILER=$SCRIPT_DIR/../../bminor

# Test to make sure our program follows mathematical idempotentness

# In mathematics and computer science, an operation or transformation is considered idempotent
# if applying it multiple times has the same effect as applying it once

# If the formatter is working correctly, the output passed through the formatter twice should be
# identical to passing it through once

# In a visual sense, were testing the following:
# a --> b --> c
# a --------> c
# ... where the arrows represent putting some program into the formatter

# Clean out all of the output files
rm -rf *.out

for FILENAME in $SCRIPT_DIR/good*.bminor; do $COMPILER -prettyprint "${FILENAME}" > "${FILENAME}.1.out" 2>&1; done

for FILENAME in $SCRIPT_DIR/good*.bminor; do $COMPILER -prettyprint "${FILENAME}" > "${FILENAME}.int.out" 2>&1; done
for FILENAME in $SCRIPT_DIR/good*.int.out; do $COMPILER -prettyprint "${FILENAME}" > "${FILENAME%.int.out}.2.out" 2>&1; done

EXIT_CODE=0

for FILE in $SCRIPT_DIR/good*.bminor; do
    if cmp -s "${FILE}.1.out" "${FILE}.2.out"; then
        echo "${FILE}.1.out and ${FILE}.2.out - IDENTICAL"
    else
        cmp "${FILE}.1.out" "${FILE}.2.out"
        EXIT_CODE=1
    fi
done

if [ "$EXIT_CODE" -eq 1 ]; then
    echo "=== Prettyprint idempotentness test FAILED ==="
fi

return $EXIT_CODE
