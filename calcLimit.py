import sys
import math
import networkx as nx
import matplotlib.pyplot as plt

G = nx.Graph()

lines = open(sys.argv[1], 'r').readlines()

N, M = [ int(x) for x in lines[0].split() ]

for line in lines[1:M+1]:
    spl = line.split()
    G.add_edge(int(spl[0]), int(spl[1]))

perm = {}

for line in lines[M+1:M+N+2]:
    spl = line.split()
    perm[int(spl[0])] = int(spl[1])

dist = nx.shortest_path_length(G)
dist = dict(dist)

lim  = -1

for conn in nx.connected_components(G):
    sum = 0

    print(conn)

    for v in conn:
        sum += dist[v][perm[v]]
        
    if lim == -1:
        lim = sum/2
    else: 
        lim = min(lim, sum/2)

print(math.floor(lim))
