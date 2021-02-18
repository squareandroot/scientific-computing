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

fig, ((ax1, ax2, ax3), (ax4, ax5, ax6)) = plt.subplots(2, 3, figsize=(21,14))

for file in source_files:
    data = np.loadtxt(file)
    parameter_string = "_".join(file.rstrip(".out").split("_")[-3:])
    label_text = "; ".join(file.rstrip(".out").split("_")[-3:])

    # 8 fins, 0.3 width, height variation
    if parameter_string in ["8_0.3_1.0", "8_0.3_1.5", "8_0.3_2.0"]:
        ax1.plot(data, label=label_text)
        ax1.legend()
        ax1.set(xlabel="t'", ylabel="Temperature [°C]", box_aspect=1)

    # 13 fins, 0.2 width, height variation
    if parameter_string in ["13_0.2_1.0", "13_0.2_1.5", "13_0.2_2.0"]:
        ax2.plot(data, label=label_text)
        ax2.legend()
        ax2.set(xlabel="t'", ylabel="Temperature [°C]", box_aspect=1)

   # 30 fins, 0.1 width, height variation
    if parameter_string in ["30_0.1_1.0", "30_0.1_1.5", "30_0.1_2.0"]:
        ax3.plot(data, label=label_text)
        ax3.legend()
        ax3.set(xlabel="t'", ylabel="Temperature [°C]", box_aspect=1)

    # 8 fins, width variation, 1.5 height
    if parameter_string in ["8_0.2_1.5", "8_0.3_1.5", "8_0.5_1.5"]:
        ax4.plot(data, label=label_text)
        ax4.legend()
        ax4.set(xlabel="t'", ylabel="Temperature [°C]", box_aspect=1)

    # 13 fins, width variation, 1.5 height
    if parameter_string in ["13_0.1_1.5", "13_0.2_1.5", "13_0.3_1.5"]:
        ax5.plot(data, label=label_text)
        ax5.legend()
        ax5.set(xlabel="t'", ylabel="Temperature [°C]", box_aspect=1)

    # best results for each number of fins
    if parameter_string in ["8_0.3_1.5", "13_0.2_1.5", "30_0.1_1.5"]:
        ax6.plot(data, label=label_text)
        ax6.legend()
        ax6.set(xlabel="t'", ylabel="Temperature [°C]", box_aspect=1)

# save entire plot
fig.savefig("temperature_plot_2x3.pdf", bbox_inches="tight", pad_inches=0.05)
fig.show()


#safe individual subplots
ax1_bbox = ax1.get_tightbbox(fig.canvas.get_renderer())
ax1_extent = ax1_bbox.transformed(fig.dpi_scale_trans.inverted())
fig.savefig("temperature_plot_8_0.3_var.pdf", bbox_inches=ax1_extent.expanded(1.01,1.01))

ax2_bbox = ax2.get_tightbbox(fig.canvas.get_renderer())
ax2_extent = ax2_bbox.transformed(fig.dpi_scale_trans.inverted())
fig.savefig("temperature_plot_13_0.2_var.pdf", bbox_inches=ax2_extent.expanded(1.01,1.01))

ax3_bbox = ax3.get_tightbbox(fig.canvas.get_renderer())
ax3_extent = ax3_bbox.transformed(fig.dpi_scale_trans.inverted())
fig.savefig("temperature_plot_30_0.1_var.pdf", bbox_inches=ax3_extent.expanded(1.01,1.01))

ax4_bbox = ax4.get_tightbbox(fig.canvas.get_renderer())
ax4_extent = ax4_bbox.transformed(fig.dpi_scale_trans.inverted())
fig.savefig("temperature_plot_8_var_1.5.pdf", bbox_inches=ax4_extent.expanded(1.01,1.01))

ax5_bbox = ax5.get_tightbbox(fig.canvas.get_renderer())
ax5_extent = ax5_bbox.transformed(fig.dpi_scale_trans.inverted())
fig.savefig("temperature_plot_13_var_1.5.pdf", bbox_inches=ax5_extent.expanded(1.01,1.01))

ax6_bbox = ax6.get_tightbbox(fig.canvas.get_renderer())
ax6_extent = ax6_bbox.transformed(fig.dpi_scale_trans.inverted())
fig.savefig("temperature_plot_best_of_num_fins.pdf", bbox_inches=ax6_extent.expanded(1.01,1.01))



