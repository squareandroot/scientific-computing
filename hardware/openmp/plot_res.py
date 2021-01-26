import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("out.txt")

fig, axs = plt.subplots(2)
axs[0].grid(True)
axs[0].semilogx(data[:,0]**2, data[:,1], "o-", color="orange")
axs[0].set(xlabel="grid size", ylabel="Mem BW [GB/s]")

axs[1].grid(True)
axs[1].loglog(data[:,0]**2, data[:,2], "o-")
axs[1].set(xlabel="grid size", ylabel="Exec time [s]")

fig.savefig("membw-exectime.pdf", bbox_inches="tight", pad_inches=0.05)
