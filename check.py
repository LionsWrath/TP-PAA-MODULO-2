import sys
import math
import networkx as nx
import matplotlib.pyplot as plt

G = nx.Graph()

lines_i = open(sys.argv[1], 'r').readlines()
lines_o = open(sys.argv[2], 'r').readlines()

# Read first file
N, M = [ int(x) for x in lines_i[0].split() ]


for line in lines_i[1:M+1]:
    spl = line.split()
    G.add_edge(int(spl[0]), int(spl[1]))

# Read second file
R, F, B, T = [ int(x) for x in lines_o[0].split() ]

# Check
conn = nx.connected_components(G)
conn = list(conn)

if R + F + B + T != len(conn):
    print("NOK")
    sys.exit()

for c in conn:
    H = G.subgraph(c)
    if nx.is_tree(H):
        one = 0

        for d in H.degree():
            if d[1] == 1:
                one += 1

        if one == 2:
            R -= 1
        else:
            F -= 1
    else:
        # Cycle, Complete Bipartite
        two = 0
        for d in H.degree():
            if d[1] == 2:
                two += 1

        if two == len(H.nodes()):
            T -= 1
        else:
            B -= 1

if R != 0:
    print("NOK", R, F, T, B)
    sys.exit()

if F != 0:
    print("NOK", R, F, T, B)
    sys.exit()

if T != 0:
    print("NOK", R, F, T, B)
    sys.exit()

if B != 0:
    print("NOK", R, F, T, B)
    sys.exit()

print("OK")
