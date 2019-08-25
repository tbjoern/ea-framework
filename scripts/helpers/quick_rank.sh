#!/bin/bash
set -e

exec_dir=${0%/*}

$exec_dir/../result_processing/rank_avg_fitness.py $1 | $exec_dir/kv_to_latex_table.sh > ${1%.*}_rank.tex

