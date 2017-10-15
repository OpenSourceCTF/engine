#!/bin/sh

find tp_maps/ -name "*.json" | while read file;
do
    base=`basename ${file} .json`
    ./tagos export "tp_maps/${base}.json" "tp_maps/${base}.png" "maps/${base}.json"
done
