#!/usr/bin/env
import csv
import sys


def keep_iteration(i):
    if i < 100:
        return True
    elif i < 1000 and i % 10 == 0:
        return True
    elif i > 1000 and i % 1000 == 0:
        return True
    return False

if __name__== "__main__":
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument("result")

    args = parser.parse_args()

    with open(args.result, 'r') as f:
        reader = csv.DictReader(f)
        reader.fieldnames = [x.strip() for x in reader.fieldnames]
        writer = csv.DictWriter(sys.stdout, fieldnames=reader.fieldnames)
        writer.writeheader()
        for row in reader:
            iteration = int(row['generation'])
            if keep_iteration(iteration):
                writer.writerow(row)

