#! /usr/bin/env python

import os
import subprocess

ASS_EXT = '.assignment'
CSV_EXT = '.csv'
START_ASSIGNMENT_BIN = './start_assignment'

def start_assignment(path, filename):
    name, extension = os.path.splitext(filename)
    input_file_path = os.path.join(path, filename)
    output_file_path = os.path.join(path, name + ASS_EXT)
    with open(output_file_path, 'w') as f:
        subprocess.run([START_ASSIGNMENT_BIN, '-i', f'{input_file_path}'], stdout=f)

def main():
    import argparse

    parser = argparse.ArgumentParser()

    parser.add_argument('instance_dir')

    args = parser.parse_args()

    if not os.path.isdir(args.instance_dir):
        print(f"Path {instance_dir} is not a directory")
        exit(1)

    for path, subdirs, files in os.walk(args.instance_dir):
        for f in files:
            if not (ASS_EXT in f or CSV_EXT in f):
                print(f)
                start_assignment(path, f)

if __name__ == "__main__":
    main()
