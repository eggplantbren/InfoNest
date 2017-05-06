# Calculate the entropy of a precisional question
# in a discrete problem.

import numpy as np

# Hypothesis space
x = np.arange(0, 10)

# Probabilities
p = np.ones(len(x)) / len(x)

# Precision of precisional question
width = 5

H = 0.0

old_interval = None
for i in range(0, len(x) - width + 1):
    left = i
    right = i + width
    interval = (x >= left) & (x < right)

    pp = p[interval]
    P = pp.sum()
    if P > 0.0 and P < 1.0:
        H += -P*np.log(P)
    overlap = None
    if i > 0:
        overlap = old_interval & interval
        pp = p[overlap]
        P = pp.sum()
        if P > 0.0 and P < 1.0:
            H -= -P*np.log(P)

    old_interval = interval

print("Entropy of central issue = {h}".format(h=-np.sum(p*np.log(p))))
print("Entropy of precisional question = {H}".format(H=H))

