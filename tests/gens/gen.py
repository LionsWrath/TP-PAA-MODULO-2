import sys
from random import randint

N = int(sys.argv[1])
M = int(sys.argv[2])
F = open(sys.argv[3] + ".gen", 'w')
L = int(sys.argv[4])
S = 0

C = []

while N > 4 and M > 0:
    t = randint(1, 4)
    v = randint(1, N) % L

    if v < 5:
        continue

    if t == 1:

        l = randint(2, v - 1) # Left
        r = v - l  # Right

        if l * r > M or r == 1:
            continue

        C.append(str(t) + " " + str(l) + " " + str(r))

        N = N - (l + r)
        M = M - l * r

    if t == 2:

        if v > M or v % 2 != 0:
            continue

        C.append(str(t) + " " + str(v))

        N = N - v
        M = M - v

    if t == 3:

        if v - 1 > M:
            continue

        C.append(str(t) + " " + str(v))

        N = N - v
        M = M - (v - 1)

    if t == 4:

        if v - 1 > M:
            continue

        C.append(str(t) + " " + str(v))

        N = N - v
        M = M - (v - 1)

    S = S + 1

F.write(str(S) + "\n")

for line in C:
    F.write(line + "\n")

F.close()
