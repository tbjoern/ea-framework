#!/usr/bin/env python3

from summary import get_summaries_and_configs, id_name_map_from_configs, merge_summary
from os.path import basename

def best_fitness(summary):
    summary_best_fitness = {}
    for instance, algo_data in summary.items():
        summary_best_fitness[instance] = {}
        for id, runs in algo_data.items():
            best_fitness = 0
            for run, data in runs.items():
                if data[0] > best_fitness:
                    best_fitness = data[0]
            summary_best_fitness[instance][id] = best_fitness
    return summary_best_fitness

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

    summary_best_fitness = best_fitness(summary)

    id_name_map = id_name_map_from_configs(configs)

    output = { 'best_fitness': summary_best_fitness, 'id_name_map': id_name_map }

    json.dump(output, sys.stdout)
    


if __name__ == '__main__':
    main()


