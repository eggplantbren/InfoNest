# Testing the analytical formula currently in the appendix
import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(-20, 20, 200001)
dx = x[1] - x[0]
p = np.exp(-0.5*x**2) / np.sqrt(2*np.pi)
L = 1.0

H = 0.0
for i in range(0, len(x)):
    left = (x >= x[i] - L) & (x <= x[i])
    P = np.sum(p[left])*dx
    H += -p[i]*dx*(1.0 + np.log(P))
    if (i+1) % 1000 == 0:
        print(i+1, H)

# grid pts  H
# 20001     0.53386
# 30001     0.53453
# 50001     0.53507
# 100001    0.53547
# 200001    0.53567
