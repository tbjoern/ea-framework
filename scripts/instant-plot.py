#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
import csv
import math
import json

labels = ['g-', 'b-', 'r-', 'y-', 'c-']

def plot(x_dataset, y_dataset, labels, legend, figure):
    for x,y,l,le in zip(x_dataset, y_dataset,labels, legend):
        figure.plot(x[0],y[0],l, label=le)

def read_config(filename):
    with open(filename, 'r') as f:
        config = json.load(f)
    return config

def init_datafield(gens, algos, run_count, constructor):
    return [[constructor(gens) for i in range(run_count)] for i in range(algos)]

def running_maximum(data):
    for run in data:
        for d in run:
            maximum = 0
            for i,v in enumerate(d):
                if v > maximum:
                    maximum = v
                else:
                    d[i] = maximum

def running_sum(data):
    for run in data:
        for d in run:
            sum = 0
            for i,v in enumerate(d):
                sum += v
                d[i] = sum

def fill_gaps(data):
    for run in data:
        for d in run:
            last = 0
            for i,v in enumerate(d):
                if v == 0:
                    d[i] = last
                else:
                    last = v

def prompt_for_axis(axes, labels):
    items = ["Select axis -"]
    for i,t in enumerate(labels):
        items.append(f"{i} {t}")
    items.append(": ")
    prompt = " ".join(items)
    number = input(prompt)
    number = int(number)
    return axes[number]
    
def main():
    import argparse

    parser = argparse.ArgumentParser()

    parser.add_argument('-c', '--config', default='./config.json')
    parser.add_argument('csv')

    args = parser.parse_args()

    config = read_config(args.config)
    algos = len(config["mutators"])
    gens = int(config["generation_count"]) + 1
    run_count = int(config["run_count"])

    plot_legend = ["" for i in range(algos)]
    for mutator in config["mutators"]:
        plot_legend[mutator["id"]] = mutator["type"]

    filename = args.csv
    fitness = init_datafield(gens, algos, run_count, np.zeros)
    time = init_datafield(gens, algos, run_count, np.zeros)
    generations = init_datafield(gens, algos, run_count, np.arange)
    objcalls = init_datafield(gens, algos, run_count, np.zeros)
    flips = init_datafield(gens, algos, run_count, np.zeros)
    with open(filename, 'r') as f:
        reader = csv.DictReader(f, delimiter=',')
        reader.fieldnames = [x.strip() for x in reader.fieldnames]
        for i,row in enumerate(reader):
            dataset = int(row['id'])
            iteration = int(row['generation'])
            run = int(row['run'])
            fitness[dataset][run][iteration] = row['fitness']
            time[dataset][run][iteration] = row['mutation_time']
            objcalls[dataset][run][iteration] = row['objcalls']
            flips[dataset][run][iteration] = row['flips']
    
    running_maximum(fitness)
    fill_gaps(time)
    fill_gaps(objcalls)
    fill_gaps(flips)

    axes = [fitness, time, objcalls, flips, generations]
    axes_labels = ["fitness", "time", "objcalls", "flips", "generations"]

    x = prompt_for_axis(axes, axes_labels)
    y = prompt_for_axis(axes, axes_labels)

    plot_labels = ['b-', 'r-', 'g-']
    
    f = plt.figure(1)
    sub = f.add_subplot(111)
    plot(x, y, plot_labels, plot_legend, sub)
    sub.legend()
    plt.show()
    
if __name__ == "__main__":
    main()
