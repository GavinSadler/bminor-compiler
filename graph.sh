#!/bin/sh

for i in $(seq 1 15); do
    echo "Graphing /tests/parser/good$i.bminor";
    ./bminor --graph ./tests/parser/good$i.bminor >./tests/parser/good$i.bminor.gv;
done

for i in $(seq 1 10); do
    echo "Graphing /tests/printer/good$i.bminor";
    ./bminor --graph ./tests/printer/good$i.bminor >./tests/printer/good$i.bminor.gv;
done

for i in $(seq 1 10); do
    echo "Graphing /tests/typecheck/good$i.bminor";
    ./bminor --graph ./tests/typecheck/good$i.bminor >./tests/typecheck/good$i.bminor.gv;
done
