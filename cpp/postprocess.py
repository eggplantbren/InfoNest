__all__ = ["postprocess"]

# Imports
import numpy as np
import matplotlib.pyplot as plt
import sys

def postprocess(tol=1E-3):
    """
    Load the output file and estimate the entropy.
    """

    # Load run info
    num_particles = np.loadtxt("run_data.txt")

    # Open output.txt
    f = open("output.txt", "r")

    # Store results in these
    counts = []
    rep_lengths = []

    while True:
        # Read values until end of file
        line = f.readline()[0:-1]
        values = line.split(" ")
        final_line = len(values) != 2

        # The values on the line
        if not final_line:
            k, v = int(values[0]), float(values[1])

        # Start a count at zero
        if k == 1:
            counts.append(0)
            rep_lengths.append(0)

        # Accumulate results
        if not final_line:
            if v >= tol:
                counts[-1] += 1
            rep_lengths[-1] += 1

        if final_line:
            break

    # Convert to numpy array
    counts = np.array(counts)
    rep_lengths = np.array(rep_lengths)

    if len(rep_lengths) > 1 and rep_lengths[-1] != rep_lengths[0]:
        counts = counts[0:-1]
        rep_lengths = rep_lengths[0:-1]
        print("# Skipping last rep as it is incomplete.")

    for c in counts:
        print(c / num_particles)

    print("# Found {a} complete reps.".format(a=len(counts)))
    print("# mean(depth) = {mc}.".format(mc=counts.mean() / num_particles))
    print("# std (depth) = {sd}.".format(sd=counts.std()  / num_particles))
    print("# sem (depth) = {sem}.".format(\
            sem=counts.std() / num_particles/np.sqrt(len(counts))))

    if np.any(counts == rep_lengths):
        print("# WARNING: A run didn't achieve the desired tolerance.")


if __name__ == "__main__":
    postprocess(tol=1E-3*np.sqrt(100.0))

