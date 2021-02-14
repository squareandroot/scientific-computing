import numpy as np
import matplotlib.pyplot as plt
import os

rootdir = "./out"
source_files = list()

for subdir, dirs, files in os.walk(rootdir):
    for file in files:
        if file.startswith("max-heat"):
            source_files.append(os.path.join(subdir, file))

source_files.sort()

fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(20,7))

fins_to_axes = {"8": ax1, "13": ax2, "30": ax3}

for file in source_files:
    data = np.loadtxt(file)
    ax = fins_to_axes[file.split("_")[-3]]
    label_text = "; ".join(file.rstrip(".out").split("_")[-3:])
    ax.plot(data, label=label_text)
    ax.legend()
    ax.set(xlabel="t'", ylabel="Temperature [°C]", box_aspect=1)

# fig.savefig("temperature_plot.pdf", bbox_inches="tight", pad_inches=0.05)
fig.show()
