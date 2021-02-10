import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("./out/max-heat.out")

plt.plot(data)
