import sys
import networkx as nx
import matplotlib.pyplot as plt

G = nx.Graph()

lines = open(sys.argv[1], 'r').readlines()

N, M = [ int(x) for x in lines[0].split() ]

for line in lines[1:M+1]:
    spl = line.split()
    G.add_edge(int(spl[0]), int(spl[1]))

nx.draw(G, with_labels=True, k=0.15, iterations=20)
plt.show()
