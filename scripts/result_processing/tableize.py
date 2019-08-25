#!/usr/bin/env python3

import os 
import json
from summary import get_summaries_and_configs, id_name_map_from_configs, merge_summary

def argmax(dct, valuefunc=lambda x: x):
    maxitem = None
    maxvalue = None
    for key,value in dct.items():
        v_c = valuefunc(value)
        if maxitem is None or maxvalue < v_c:
            maxitem = key
            maxvalue = v_c
    return (maxitem, maxvalue)

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('dirs', nargs='*')
    args = parser.parse_args()

    if len(args.dirs) == 0:
        exit(1)

    summaries,configs = get_summaries_and_configs(args.dirs)

    summary = merge_summary(*summaries)

    id_name_map = id_name_map_from_configs(configs)

    max_id = max(id_name_map.keys())

    table_begin = [r'\begin{longtable}{l | '] + ['c' for i in range(max_id + 1)] + ['}']
    table_begin = ' '.join(table_begin)
    print(table_begin)
    print('\hline')
    print(r'\endhead')

    for instance, algos in sorted(summary.items(), key=lambda x: x[0]):
        instance_name, _ = os.path.splitext(instance)
        instance_name = instance_name.replace('_', '-')
        line = [instance_name] + ['0' for i in range(max_id + 1)]
        maxitem,_ = argmax(algos, valuefunc=lambda x: int(x['fitness']))
        for id, data in algos.items():
            if id == maxitem:
                line[int(id) + 1] = r'\textcolor{red}{' + str(data['fitness']) + '}'
            else:
                line[int(id) + 1] = str(data['fitness'])
        print('&'.join(line) + r'\\')

    print(r'\end{longtable}')


if __name__ == '__main__':
    main()


