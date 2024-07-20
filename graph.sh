#!/bin/bash

for i in $(seq 1 15); do ./bminor --graph ./tests/parser/good$i.bminor >./tests/parser/good$i.bminor.gv; done

for i in $(seq 1 10); do ./bminor --graph ./tests/printer/good$i.bminor >./tests/printer/good$i.bminor.gv; done

for i in $(seq 1 10); do ./bminor --graph ./tests/typecheck/good$i.bminor >./tests/typecheck/good$i.bminor.gv; done
