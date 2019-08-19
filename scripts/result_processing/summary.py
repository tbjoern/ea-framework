#!/usr/bin/env python3

import sys
import json
import os
import csv
import logging
import multiprocessing as mp

def add_params_of_type(algo_obj, tp):
    tokens = [tp]
    if 'pmut' in tp or 'fmut' in tp:
        tokens.append('_' + str(algo_obj['arguments']['power_law_beta']))
    if 'pmutActivity' == tp or 'unifSigmoid' == tp:
        tokens.append('_' + str(algo_obj['arguments']['decay_rate']))
    return ''.join(tokens)

def get_mutator_name(mutator):
    raw_name = mutator['type']
    return add_params_of_type(mutator, raw_name)

def get_algorithm_name(algorithm):
    raw_name = algorithm['name']
    return add_params_of_type(algorithm, raw_name)

def make_subset(to_trim, superset):
    trimm_keys = []
    for key in to_trim:
        if key not in superset:
            logging.info(f"removing {key}")
            trimm_keys.append(key)
    for key in trimm_keys:
        to_trim.pop(key)

def merge_summary(*args):
    merged = args[0]
    for d in args[1:]:
        make_subset(d, merged)
        make_subset(merged, d)
        for filename, algo_data in d.items():
            for algo, gen_info in algo_data.items():
                if algo in merged[filename]:
                    logging.error(str(algo) + " exists twice")
                merged[filename][algo] = gen_info
    return merged

def get_summaries_and_configs(dirs):
    summaries = []
    configs = []
    for d in dirs:
        config_path = os.path.join(d, d + '.json')
        summary_path = os.path.join(d, 'summary.json')
        with open(config_path, 'r') as c:
            config = json.load(c)
        with open(summary_path, 'r') as s:
            summary = json.load(s)
        configs.append(config)
        summaries.append(summary)
    return (summaries, configs)

def id_name_map_from_configs(configs):
    id_name_map = {}
    for c in configs:
        if 'mutators' in c:
            for mutator in c['mutators']:
                id = int(mutator['id'])
                id_name_map[id] = get_mutator_name(mutator)
        if 'algorithms' in c:
            for algorithm in c['algorithms']:
                id = int(algorithm['id'])
                id_name_map[id] = get_algorithm_name(algorithm)
    return id_name_map

def read_instance_data(instance, time_limit=None):
    logging.info("reading " + instance)
    data = {}
    with open(instance, 'r') as f:
        reader = csv.DictReader(f)
        if reader.fieldnames is None:
            return None
        reader.fieldnames = [x.strip() for x in reader.fieldnames]
        for row in reader:
            algorithm = int(row['id'])
            run = int(row['run'])
            if not algorithm in data:
                data[algorithm] = {}
            if not run in data[algorithm]:
                data[algorithm][run] = { 'fitness': None, 'time' : 0 }
            fitness = int(float(row['fitness']))
            generation = int(row['generation'])
            time = int(float(row['mutation_time']))
            if time_limit is not None and time > time_limit:
                continue
            if data[algorithm][run]['fitness'] is None or data[algorithm][run]['fitness'] < fitness:
                data[algorithm][run] = {
                        'fitness': fitness,
                        'generation': generation,
                        'time': time + data[algorithm][run]['time'] # data is consecutive, thats why this works
                }
    return (instance,data)

def walk_result_dir(result_dir, debug=False, time_limit=None):
    all_files = []
    for path, subdirs, files in os.walk(result_dir):
        for f in files:
            if '.csv' in f:
                all_files.append(os.path.join(path,f))
    result_data = {}
    with mp.Pool(2) as p:
        pool_data = p.map(read_instance_data, all_files, time_limit)
    result_data = {os.path.splitext(os.path.basename(filename))[0]:data for filename, data in (d for d in pool_data if d is not None)}
    return result_data
        
def main():
    import argparse

    parser = argparse.ArgumentParser()

    parser.add_argument('result_dir')
    parser.add_argument('--debug', action='store_true')
    parser.add_argument('--timelimit', type=int)

    args = parser.parse_args()

    if args.debug:
        logging.basicConfig(filename='summarize.log', level=logging.DEBUG)

    result_data = walk_result_dir(args.result_dir, debug=args.debug, time_limit=args.timelimit)

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
