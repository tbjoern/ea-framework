#!/bin/bash
set -e

execdir=${0%/*}

for d in $(ls | grep "1M_10m")
do
    if [ -d $d ]
    then
        $execdir/clean_summary.py $d/summary.json > $d/summary_clean.json
    fi
done
