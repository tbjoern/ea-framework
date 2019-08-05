#! /bin/bash

instance_dir=$1
exec_name='python3 ../scripts/compress_run_results.py'

result_dir="${instance_dir}_cmpr"
echo "using output directory: $result_dir"

#install -d $result_dir

for file in `find -L $instance_dir -type f | grep .csv`
do
    ext=${file##*.}
    fname=`basename $file .$ext`
    fdir=`dirname $file`
    logfile="$fdir/$fname.csv"
    cmd="${exec_name} ${file}"
    echo $cmd
    install -D -m 644 /dev/null "$result_dir/${logfile}"
    python3 ../scripts/compress_run_results.py $file > $result_dir/${logfile}
done

