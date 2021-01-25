import numpy as np
import matplotlib.pyplot as plt

fhand = open("ReuleauxCardioid.msh", "r")

meshfile = list()
groups = dict()
count = 0

for line in fhand:
    meshfile.append(line.rstrip())
    if line.startswith("$"):
        groups[line.lstrip("$").rstrip()] = count
    count += 1

fhand.close()

nodes = meshfile[groups["Nodes"]+2:groups["EndNodes"]]

edges = list()
triangles = list()

for i in range(groups["Elements"]+2, groups["EndElements"]):
    line = [int(x) for x in meshfile[i].split()]
    if line[1] == 1:
        edges.append(line)
    elif line[1] == 2:
        triangles.append(line)
    else:
        break

for i, line in enumerate(nodes):
    nodes[i] = [float(x) for x in line.split()]

nodes = np.array(nodes)
edges = np.array(edges, dtype=int)
triangles = np.array(triangles, dtype=int)

plt.figure('Triangles')

for triangle in triangles:
    x1 = nodes[triangle[-3]-1][-3]
    x2 = nodes[triangle[-2]-1][-3]
    x3 = nodes[triangle[-1]-1][-3]

    y1 = nodes[triangle[-3]-1][-2]
    y2 = nodes[triangle[-2]-1][-2]
    y3 = nodes[triangle[-1]-1][-2]

    triangle_x = [x1, x2, x3]
    triangle_y = [y1, y2, y3]

    plt.plot(triangle_x, triangle_y, "o-", color="orange")

for edge in edges:
    x1 = nodes[edge[-2]-1][-3]
    x2 = nodes[edge[-1]-1][-3]

    y1 = nodes[edge[-2]-1][-2]
    y2 = nodes[edge[-1]-1][-2]

    edge_x = [x1, x2]
    edge_y = [y1, y2]

    if edge[3] == 1:
        plt.plot(edge_x, edge_y, "o-", color="blue")
    elif edge[3] == 2:
        plt.plot(edge_x, edge_y, "o-", color="green")

plt.show()
