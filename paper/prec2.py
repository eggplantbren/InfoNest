# Testing the analytical formula currently in the appendix
import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(-10, 10, 50001)
dx = x[1] - x[0]
p = np.exp(-0.5*x**2) / np.sqrt(2*np.pi)
p[np.abs(x) > 5] = 0
L = 1.0

H = 0.0
for i in range(0, len(x)):
    left = (x >= x[i] - L) & (x <= x[i])
    P = np.sum(p[left])*dx
    if p[i] > 0:
        H += -p[i]*dx*(1 + np.log(P))
    if (i+1) % 1000 == 0:
        print(i+1, H)

