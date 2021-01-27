import numpy as np
import matplotlib.pyplot as plt

path = "./out/"

time_steps = 100

plot_times = [0, 0.05, 0.1, 0.25, 1]

fileNo = [round(t*(time_steps-1)+1) for t in plot_times]

files = list()

for no in fileNo:
    files.append(np.loadtxt(path + "out-neumann-t{}.dat".format(no)))


plt.figure(figsize=(8, 3))

for i, T in enumerate(files):
    plt.plot(T[:, 0], T[:, 1], label="T({}, x')".format(plot_times[i]))
plt.xlim([0, 1])
plt.legend()
plt.xlabel("x'")
plt.ylabel("temperature [K]")

plt.savefig("T-nd-plot.pdf", bbox_inches="tight", pad_inches=0.05)


plt.figure(figsize=(8, 3))

times_label = ["0 min", "8 min", "17 min", "42 min", "2.8 h"]

for i, T in enumerate(files):
    plt.plot(0.5*T[:, 0], T[:, 1], label="T({}, x)".format(times_label[i]))
plt.xlim([0, 0.5])
plt.legend()
plt.xlabel("x [m]")
plt.ylabel("temperature [K]")

plt.savefig("T-iron-rod-plot.pdf", bbox_inches="tight", pad_inches=0.05)


plt.figure(figsize=(8, 3))

times_label = ["0 h", "42 h", "3.5 days", "9 days", "35 days"]

for i, T in enumerate(files):
    plt.plot(0.5*T[:, 0], T[:, 1], label="T({}, x)".format(times_label[i]))
plt.xlim([0, 0.5])
plt.legend()
plt.xlabel("x [m]")
plt.ylabel("temperature [K]")

plt.savefig("T-wooden-rod-plot.pdf", bbox_inches="tight", pad_inches=0.05)
