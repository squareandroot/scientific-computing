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

for file in source_files:
    data = np.loadtxt(file)
    label_text = "; ".join(file.rstrip(".out").split("_")[-3:])
    plt.plot(data, label=label_text)

plt.legend()
plt.title("Maximum temperature")
plt.xlabel("t'")
plt.ylabel("Temperature [°C]")
# plt.savefig("temperature_plot.pdf", bbox_inches="tight", pad_inches=0.05)
plt.show()
