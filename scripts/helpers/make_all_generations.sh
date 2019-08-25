#!/bin/bash

set -e

dirs=$*

for d in ${dirs[@]}
do
    if [ ! -d $d ]
    then
        echo "$d is not a directory"
        exit 1
    fi
done

for d in ${dirs[@]}
do
    echo "$d"
    echo "Ranking"
    ./scripts/helpers/batch.sh $d .json ./scripts/helpers/quick_rank_generations.sh
    echo "Plotting"
    ./scripts/helpers/batch.sh $d .json ./scripts/helpers/quick_plot_generations.sh
done
