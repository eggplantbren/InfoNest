__all__ = ["postprocess"]

# Imports
import numpy as np
import matplotlib.pyplot as plt

def postprocess(num_particles=1, tol=1E-3):
    """
    Load the output file and estimate the entropy.
    """

    # Load output file
    output = np.loadtxt("output.txt")
    num_particles = np.loadtxt("num_particles.txt")

    # Find the starts of the runs
    start_indices = np.nonzero(output[:,0] == 1)[0]

    reps = len(start_indices)
    counts = np.empty(reps)
    for i in range(0, reps):
        start_index = start_indices[i]
        if i < reps - 1:
            run = output[start_index : start_indices[i+1], :]
        else:
            run = output[start_index : , :]
        print("Processed run {k}/{N}.".format(k=i+1, N=reps))
        counts[i] = (run[:,1] > tol).sum()

    print("mean(counts) = {mc}.".format(mc=counts.mean()))
    print("std (counts) = {sd}.".format(sd=counts.std()))

    # Estimate of the differential entropy
    # log(probability) ~= log(2 * tol * density)
    H = counts.mean() / num_particles + np.log(2 * tol)
    stderr = (counts.std() / num_particles) / np.sqrt(reps)

    print("H = {H} +- {stderr} nats.".format(H=H, stderr=stderr))

#    plt.hist(mean_depth, 100, color=[0.2, 0.2, 0.2])
#    plt.show()

if __name__ == "__main__":
    postprocess()

