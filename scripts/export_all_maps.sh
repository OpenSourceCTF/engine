#!/bin/sh

echo "" > export_all_maps.failed.log
find ./tp_maps/ -name "*.json" | while read file;
do
    base=`basename ${file} .json`
    ./tagos export "./tp_maps/${base}.json" "./tp_maps/${base}.png" "./maps/${base}.json" || echo ${base} >> export_all_maps.failed.log
done
