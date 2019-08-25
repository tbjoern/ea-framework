#!/bin/bash
set -e

exec_dir=${0%/*}

$exec_dir/../result_processing/plot_avg_fitness.py --image ${1%.*}.png $1

