# A little script for mutual information calculations
import matplotlib.pyplot as plt
import numpy as np

# Name of the problem
name = "groupdiff"
results1 = np.loadtxt(name + "1.txt")
results2 = np.loadtxt(name + "2.txt")
results3 = np.loadtxt(name + "3.txt")

L = min([len(results1), len(results2), len(results3)])
I = results1[0:L] + results2[0:L] - results3[0:L]

plt.plot(I)
print("I = " + str(I.mean()) + " +- " + str(I.std()/np.sqrt(L)))
plt.show()

