#!/usr/bin/env python3

import sys
import json
import os
import csv
import logging
import multiprocessing as mp

def read_instance_data(instance, time_limit=None):
    logging.info("reading " + instance)
    data = {}
    with open(instance, 'r') as f:
        reader = csv.reader(f)
        next(reader)
        for row in reader:
            algorithm = int(row[0])
            run = int(row[1])
            if not algorithm in data:
                data[algorithm] = {}
            if not run in data[algorithm]:
                data[algorithm][run] = None
            generation = int(row[2])
            if data[algorithm][run] is None or data[algorithm][run] < generation:
                data[algorithm][run] = generation
    return (instance,data)

def walk_result_dir(result_dir, time_limit=None):
    all_files = []
    for path, subdirs, files in os.walk(result_dir):
        for f in files:
            if '.csv' in f:
                all_files.append(os.path.join(path,f))
    result_data = {}
    with mp.Pool(4) as p:
        pool_data = p.map(read_instance_data, all_files, time_limit)
    result_data = {os.path.splitext(os.path.basename(filename))[0]:data for filename, data in (d for d in pool_data if d is not None)}
    return result_data
        
def main():
    import argparse

    parser = argparse.ArgumentParser()

    parser.add_argument('result_dir')
    parser.add_argument('--debug', action='store_true')

    args = parser.parse_args()

    if args.debug:
        logging.basicConfig(filename='summarize.log', level=logging.DEBUG)

    result_data = walk_result_dir(args.result_dir) 

    json.dump(result_data, sys.stdout)

if __name__ == "__main__":
    main()



# output :
# data = {
#     "instance_name": {
#         0: {
#             0 : {
#                 'fitness': 12314,
#                 'generation' : 543,
#                 'time' : 152
#             },
#             1 : { ...
#             },
#             ...
#         },
#         1: {
#             ...
#         },
#         ...
#     },
#     ..
# }
