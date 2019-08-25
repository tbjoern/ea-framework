#!/usr/bin/env python3

import json
import logging
import sys
from summary import merge_summary

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('files', nargs='*')
    args = parser.parse_args()

    data = []
    for f in args.files:
        with open(f, 'r') as f:
            data.append(json.load(f))

    merged = merge_summary(*data)
    json.dump(merged, sys.stdout)

if __name__ == '__main__':
    main()

