# Calculate log volume of an n-ball
import numpy as np
from scipy.special import gammaln

# log-volume of n-ball
def log_volume(n, L):
    return 0.5*n*np.log(np.pi) - gammaln(0.5*n+1) + n*np.log(L)

# What I use for normal example
print(log_volume(1, 1E-3))
print(log_volume(100, 1E-3*np.sqrt(100)))
print(log_volume(101, 1E-3*np.sqrt(101)))

