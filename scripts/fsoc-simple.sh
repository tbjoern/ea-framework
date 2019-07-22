#!/bin/bash

fsoc_processor_count=60
instance_dir=$1
config=$2
cpus=$3
nodes=$4
name=`basename $config .json`
((tasks = fsoc_processor_count / cpus))

sbatch --exclusive --nodes $nodes --cpus-per-task $cpus --job-name ea-$name --ntasks-per-node $tasks ./fsoc.sh $instance_dir $config

