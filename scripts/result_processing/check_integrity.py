#!/usr/bin/env python3

from summary import get_summaries_and_configs, id_name_map_from_configs, merge_summary
from os.path import basename

def main():
    import argparse
    import json
    parser = argparse.ArgumentParser()
    parser.add_argument('dirs', nargs='*')
    args = parser.parse_args()

    if len(args.dirs) == 0:
        exit(1)

    summaries,configs = get_summaries_and_configs(args.dirs)

    first = summaries[0]
    second = summaries[1]

    for key in first.keys():
        if key not in second:
            print(f"{key} not in {args.dirs[1]}")

    for key in second.keys():
        if key not in first:
            print(f"{key} not in {args.dirs[0]}")

    print(f"{args.dirs[0]} - {len(first.keys())}")
    print(f"{args.dirs[1]} - {len(second.keys())}")
    

if __name__ == '__main__':
    main()


