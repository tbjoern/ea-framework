#!/usr/bin/env python3

import os
import json
import sys

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('file')
    args = parser.parse_args()

    with open(args.file, 'r') as f:
        data = json.load(f)

    new_data = {}
    for k,v in data.items():
        new_k = os.path.splitext(os.path.basename(k))[0]
        new_data[new_k] = v

    json.dump(new_data, sys.stdout)

if __name__ == "__main__":
    main()
