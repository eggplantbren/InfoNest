import matplotlib.pyplot as plt
import numpy as np
import numpy.random as rng

N = 100

rng.seed(0)
for i in range(0, 2000):
    alpha = np.exp(rng.randn())
    us = rng.rand(N)
    xs = (1.0 - us)**(-1.0 / alpha)

    first_half = xs[0:(N//2)]
    second_half = xs[(N//2):]

    plt.plot(np.log(first_half.mean()), np.log(second_half.mean()), "k.",
             markersize=2, alpha=0.1)
    print(i+1)

plt.axis("square")
plt.xlim([0.0, 10.0])
plt.ylim([0.0, 10.0])
plt.xlabel(r"$\ln(y_{\rm tot})$")
plt.ylabel(r"$\ln(z_{\rm tot})$")
plt.savefig("pareto.pdf", bbox_inches="tight")
plt.show()

