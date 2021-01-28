import numpy as np
import matplotlib.pyplot as plt

A = np.loadtxt("out.dat")

x = np.linspace(0, 1, len(A[0]))
t = np.linspace(0, 1, len(A))

X, Y = np.meshgrid(x, t)
Z = A

plt.figure()
ax = plt.axes(projection='3d')
ax.plot_surface(X, Y, Z, rstride=1, cstride=1,
                cmap='plasma', edgecolor='none')
ax.view_init(22.432, 63.689)
ax.set_xlabel('x')
ax.set_ylabel('t')
ax.set_zlabel('Temperature')
ax.set_title('Explicit Euler')
plt.savefig('out.pdf')
