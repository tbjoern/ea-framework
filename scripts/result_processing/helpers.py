import os
import json

def get_summaries_and_configs(dirs):
    summaries = []
    configs = []
    for d in dirs:
        config_path = os.path.join(d, d + '.json')
        summary_path = os.path.join(d, 'summary.json')
        with open(config_path, 'r') as c:
            config = json.load(c)
        with open(summary_path, 'r') as s:
            summary = json.load(s)
        configs.append(config)
        summaries.append(summary)
    return (summaries, configs)

def id_name_map_from_configs(configs):
    id_name_map = {}
    for c in configs:
        if 'mutators' in c:
            for mutator in c['mutators']:
                id = int(mutator['id'])
                id_name_map[id] = get_mutator_name(mutator)
        if 'algorithms' in c:
            for algorithm in c['algorithms']:
                id = int(algorithm['id'])
                id_name_map[id] = get_algorithm_name(algorithm)
    return id_name_map
