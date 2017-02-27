# A little script for mutual information calculations
import matplotlib.pyplot as plt
import numpy as np
from scipy.special import gammaln

# log-volume of n-ball
def log_volume(n, e):
    return 0.5*n*np.log(np.pi) - gammaln(0.5*n+1) + n*np.log(e)

# Name of the problem
name = "normal"
results1 = np.loadtxt("results/" + name + "1.txt")
results2 = np.loadtxt("results/" + name + "2.txt")
results3 = np.loadtxt("results/" + name + "3.txt")

# Parameter and data dimensions
dim = [1, 100]
dim.append(sum(dim))

# Tolerances used
tol = [1E-3, 1E-3, 1E-3]

# log-volumes
lvs = [ log_volume(dim[i], tol[i]) for i in range(0, 3) ]

L = min([len(results1), len(results2), len(results3)])
I =   (results1[0:L] + lvs[0]) \
    + (results2[0:L] + lvs[1]) \
    - (results3[0:L] + lvs[2])

plt.plot(I)
plt.axhline(0.0, linestyle="--", alpha=0.3)
print("I = " + str(I.mean()) + " +- " + str(I.std()/np.sqrt(L)) + " nats.")
plt.show()

