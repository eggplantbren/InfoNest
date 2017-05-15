# Testing the analytical formula currently in the appendix
import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(-10, 10, 200001)
dx = x[1] - x[0]
p = x + 5.0
p[np.abs(x) > 5] = 0
L = 1.0
p /= p.sum()*dx
plt.plot(x, p)
plt.show()

H = 0.0
for i in range(0, len(x)):
    left = (x >= x[i]) & (x <= x[i] + L)
    P = np.sum(p[left])*dx

    # Alternative version uses
    # right = (x >= x[i]) & (x <= x[i] + L)
    # Instead of left

    if p[i] > 0:
        H += -p[i]*dx*(1 + np.log(P))
    if (i+1) % 1000 == 0:
        print(i+1, H)

