#!/bin/bash
set -e

folder_groups="undir_empty undir_full undir_random"
select_dir=$1
select=$(ls $select_dir)

for i in ${select[@]}
do
    if [ ! -d ${i%.*} ]
    then
        mkdir ${i%.*}
    fi
done

for f in ${folder_groups[@]}
do
    ls_out=$(ls | grep _$f)
    dirs=""
    for d in ${ls_out[@]}
    do
        if [ -d $d ]
        then
            dirs="$dirs $d"
        fi
    done
    echo $dirs
    for i in ${select[@]}
    do
        ./scripts/result_processing/avg_fitness.py -i $select_dir/$i $dirs > ${i%.*}/$f.json
    done
    echo "-----"
done
