import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

data = pd.read_csv("accuracy.csv")

# Values of num_particles used in the runs
num_particles = list(set(data["num_particles"]))

for n in num_particles:
    which = data["num_particles"] == n
    x = data["mcmc_steps"][which]
    y = data["mean"][which]
    s = data["sem"][which]
    plt.errorbar(x, y, yerr=s, fmt="o", alpha=0.5)

plt.gca().set_xscale("log")
plt.show()

