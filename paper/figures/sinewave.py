from plot_utils import rc
import numpy as np
import numpy.random as rng
import matplotlib.pyplot as plt

# Set rng seed
rng.seed(0)

# Set default fonts etc
rc()

# True signal
def signal(t):
    return np.sin(2 * np.pi * t / 0.3)

# Number of data points
n = 101

# i-values as defined in the paper (i.e., starting from 1)
i = np.arange(0, n) + 1

# Two observing strategies
t_even   = (i - 1) / (n - 1)
t_uneven = ((i - 0.5) / n)**3

# 'Continuous' time
t = np.linspace(0.0, 1.0, 1001)

# Data
y_even   = signal(t_even) + 0.1*rng.randn(n)
y_uneven = signal(t_uneven) + 0.1*rng.randn(n)
y_smooth = signal(t)

plt.plot(t, y_smooth, "k", label="True signal", alpha=0.5)
plt.errorbar(t_even, y_even, color="orange", fmt="o", yerr=0.1,
             label="Even data", alpha=0.3)
plt.errorbar(t_uneven, y_uneven, color="green", fmt="o", yerr=0.1,
             label="Uneven data", alpha=0.3)
plt.xlabel("$t$", fontsize=16)
plt.ylabel("$y$", fontsize=16)
plt.ylim([-1.3, 2.4])
plt.legend(loc="upper left")
plt.savefig("sinewave.pdf", bbox_inches="tight")
plt.show()

