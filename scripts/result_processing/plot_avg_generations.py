#!/usr/bin/env python
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
import json

fmts = ['x', 'o', 's', '^', 'D', '.']
colors = ['red', 'black', 'orange', 'deepskyblue', 'limegreen', 'blueviolet']

def to_percentages(iterable):
    max_value = None
    for i in iterable:
        if max_value is None or i > max_value:
            max_value = i
    return {value:value/max_value for value in iterable}

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('avg_generations_json')
    parser.add_argument('--image')
    args = parser.parse_args()

    with open(args.avg_generations_json, 'r') as f:
        input = json.load(f)

    avg_generations = input["avg_generations"]
    id_name_map = input["id_name_map"]
    max_id = max([int(x) for x in id_name_map.keys()])

    x_axis = sorted(avg_generations.keys())
    y_axes = [[] for _ in range(max_id + 1)]
    labels = [p[1] for p in sorted(id_name_map.items(), key=lambda x: int(x[0]))]
    fmt = []
    color = []
    last_prefix = "greedy"
    color_i = 0
    fmt_i = 0 
    for label in labels:
        prefix = label.split('_')[0]
        if prefix != last_prefix:
            color_i += 1
            fmt_i = 0
            last_prefix = prefix
        else:
            fmt_i += 1
        fmt.append(fmts[fmt_i])
        color.append(colors[color_i])

    for instance in x_axis:
        algo_data = avg_generations[instance]
        value_list = [int(x[1]) for x in sorted(algo_data.items(), key=lambda x: x[0])]
        percentage_map = to_percentages(value_list[1:])
        for id, generations in algo_data.items():
            id = int(id)
            if id == 0:
                continue
            pct = percentage_map[int(generations)]
            y_axes[id].append(pct)

    f = plt.figure(figsize=(20,6), dpi=80)
    subplt = f.add_subplot(111)
    #subplt.tick_params(axis='x', labelrotation=90)
    skip = True
    for y_axis, label, fmt, color in zip(y_axes, labels, fmt, color):
        if skip:
            skip = False
            continue
        subplt.plot(x_axis, y_axis, fmt, label=label, color=color)

    subplt.set_xticklabels([])

    subplt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0.)
    plt.tight_layout()
    if args.image is not None:
        plt.savefig(args.image)
    else:
        plt.show()

if __name__ == '__main__':
    main()
        
