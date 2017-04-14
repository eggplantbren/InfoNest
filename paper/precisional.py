# Calculate the entropy of a precisional question
# in a discrete problem.

import numpy as np

# Hypothesis space
x = np.arange(0, 10)

# Probabilities
p = np.ones(len(x)) / len(x)

# Precision of precisional question
precision = 3

# Entropy of central issue
H_ci = -np.sum(p*np.log(p))
print("H_ci = {H} nats".format(H=H_ci))

H = 0.0

# Downset terms
for i in range(0, len(x) - precision + 1):
    p_subset = p[i:i+precision].sum()
    H += -p_subset * np.log(p_subset)

# Subtract overlap terms
for i in range(1, len(x) - precision + 1):
    p_subset = p[i:i+precision-1].sum()
    H -= -p_subset * np.log(p_subset)

print("H_precisional = {H} nats".format(H=H))
