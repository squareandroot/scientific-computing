import numpy as np
import matplotlib.pyplot as plt
import os

# subdir_name = "implicit/out/"
subdir_name = "explicit/out/"

directory = os.getcwd() + "/" + subdir_name


def step_from_filename(filename):
    step = filename[5:-4]
    return int(step)


time_to_plot = [0, 0.03, 0.1, 0.5, 1]
steps_from_dat = list()
T_list = list()

for file in os.listdir(directory):
    if file.endswith(".dat"):
        steps_from_dat.append(step_from_filename(file))

t_steps = np.max(steps_from_dat)
file_indicies = list()

for time in time_to_plot:
    file_indicies.append(np.argmin(np.abs(steps_from_dat/t_steps - time)))

steps_from_dat = np.array(steps_from_dat)[file_indicies]

for step in steps_from_dat:
    file = "out-t" + str(step) + ".dat"
    T_list.append(np.loadtxt(subdir_name + file))

for i, T in enumerate(T_list):
    (m, n) = (int(np.sqrt(len(T[:, 2]))), int(np.sqrt(len(T[:, 2]))))
    xlist = np.reshape(T[:, 0], (m, n))
    ylist = np.reshape(T[:, 1], (m, n))
    zlist = np.reshape(T[:, 2], (m, n))
    fig, ax = plt.subplots(1, 1)
    cp = ax.contourf(xlist, ylist, zlist, cmap="plasma")
    fig.colorbar(cp)
    ax.set_title(r"Temperature at $t \approx {}$".format(time_to_plot[i]))
    ax.set_xlabel("x'")
    ax.set_ylabel("y'")
    plt.savefig(
        "plot-t-{}.pdf".format(time_to_plot[i]), bbox_inches="tight", pad_inches=0.05)
