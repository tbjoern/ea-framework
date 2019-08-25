#!/usr/bin/env python

import json
import sys

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('avg_fitness_json')
    args = parser.parse_args()

    with open(args.avg_fitness_json, 'r') as f:
        input = json.load(f)

    avg_fitness = input["avg_fitness"]
    id_name_map = input["id_name_map"]
    max_id = max([int(x) for x in id_name_map.keys()])

    ranks = {i:[] for i in range(max_id + 1)}

    for instance, algo_data in avg_fitness.items():
        ranked_fitness = sorted(algo_data.items(), key=lambda x: x[1], reverse=True)
        rank = 1
        cur_fitness = ranked_fitness[0][1]
        for id, fitness in ranked_fitness:
            if fitness < cur_fitness:
                rank += 1
                cur_fitness = fitness
            id = int(id)
            ranks[id].append(rank)

    average_ranks = {}
    for i, rank_a in ranks.items():
        if len(rank_a) == 0:
            print(f"{id_name_map[i]} has no ranks")
        name = id_name_map[str(i)]
        average_ranks[name] = sum(rank_a)/len(rank_a)

    sorted_average_ranks = sorted(average_ranks.items(), key=lambda x: x[1])
    for name, rank in sorted_average_ranks:
        print(f"{name}:{rank:.2f}")
    

if __name__ == '__main__':
    main()
        
