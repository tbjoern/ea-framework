#!/usr/bin/env python3

from summary import get_summaries_and_configs, id_name_map_from_configs, merge_summary
from os.path import basename

def avg_fitness(summary):
    summary_avg_fitness = {}
    for instance, algo_data in summary.items():
        if len(algo_data) == 0:
            continue
        summary_avg_fitness[instance] = {}
        for id, runs in algo_data.items():
            avg_fitness = 0
            data_points = 0
            for run, data in runs.items():
                avg_fitness += data[0]
                data_points += 1
            avg_fitness /= data_points
            summary_avg_fitness[instance][id] = avg_fitness
    return summary_avg_fitness

def select(include, summary):
    with open(include, 'r') as f:
        instances_to_include = set(
                basename(x.strip())
                for x in f.readlines()
        )
    for k in list(summary.keys()):
        if k not in instances_to_include:
            summary.pop(k)

def main():
    import argparse
    import sys
    import json
    parser = argparse.ArgumentParser()
    parser.add_argument('dirs', nargs='*')
    parser.add_argument('-i', '--include')
    args = parser.parse_args()

    if len(args.dirs) == 0:
        exit(1)

    summaries,configs = get_summaries_and_configs(args.dirs)

    summary = merge_summary(*summaries)

    if args.include is not None:
        select(args.include, summary)

    summary_avg_fitness = avg_fitness(summary)

    id_name_map = id_name_map_from_configs(configs)

    output = { 'avg_fitness': summary_avg_fitness, 'id_name_map': id_name_map }

    json.dump(output, sys.stdout)
    


if __name__ == '__main__':
    main()


