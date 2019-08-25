#!/usr/bin/env python

import json
import sys

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('avg_generations_json')
    args = parser.parse_args()

    with open(args.avg_generations_json, 'r') as f:
        input = json.load(f)

    avg_generations = input["avg_generations"]
    id_name_map = input["id_name_map"]
    max_id = max([int(x) for x in id_name_map.keys()])

    ranks = {i:[] for i in range(max_id + 1)}

    for instance, algo_data in avg_generations.items():
        ranked_generations = sorted(algo_data.items(), key=lambda x: x[1], reverse=True)
        for i, (id, generations) in enumerate(ranked_generations):
            if int(id) == 0:
                del ranked_generations[i]
                break
        rank = 1
        cur_generations = ranked_generations[0][1]
        for id, generations in ranked_generations:
            if generations < cur_generations:
                rank += 1
                cur_generations = generations
            id = int(id)
            ranks[id].append(rank)

    average_ranks = {}
    for i, rank_a in ranks.items():
        if len(rank_a) == 0:
            continue
        name = id_name_map[str(i)]
        average_ranks[name] = sum(rank_a)/len(rank_a)

    sorted_average_ranks = sorted(average_ranks.items(), key=lambda x: x[1])
    for name, rank in sorted_average_ranks:
        print(f"{name}:{rank:.2f}")
    

if __name__ == '__main__':
    main()
        
