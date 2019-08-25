#!/bin/bash

set -e

dir=$1
extension=$2
cmd=$3
shift 3 || exit 1

if [ ! -d $dir ]
then
    echo "$dir is not a directory"
    exit 1
fi

for f in $(ls $dir | grep $extension)
do
    $cmd $dir/$f 
done

