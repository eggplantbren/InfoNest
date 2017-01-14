__all__ = ["postprocess"]

# Imports
import numpy as np

def postprocess(num_particles=1, tol=1E-3):
    """
    Load the output file and estimate the entropy.
    """

    # Load output file
    output = np.loadtxt("output.txt")
    num_particles = np.loadtxt("num_particles.txt")

    # Count the number of reps
    reps = np.sum(output[:,0] == 1)

    # Mean count above `tol`
    mean_count = np.sum(output[:,1] > tol) / reps
    mean_depth = mean_count / num_particles

    # Estimate of the differential entropy
    # log(probability) ~= log(2 * tol * density)
    H = mean_depth + np.log(2 * tol)

    print("Found {r} reps.".format(r = int(reps)))
    print("H = {H} nats".format(H=H))

if __name__ == "__main__":
    postprocess()

