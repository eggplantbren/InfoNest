__all__ = ["postprocess"]

# Imports
import numpy as np

def postprocess(tol=1E-3):
    """
    Load the output file and estimate the entropy.
    """

    # Load output file
    output = np.loadtxt("output.txt")

    # Count the number of reps
    reps = np.sum(output[:,0] == 1)

    # Mean count above `tol`
    mean_count = np.sum(output[:,1] > tol) / reps

    # Estimate of the differential entropy
    # log(probability) ~= log(2 * tol * density)
    H = mean_count + np.log(2 * tol)

    print("H = " + str(H) + " nats.")

if __name__ == "__main__":
    postprocess()
