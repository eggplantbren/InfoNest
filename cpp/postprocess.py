__all__ = ["postprocess", "my_loadtxt"]

# Imports
import numpy as np
import matplotlib.pyplot as plt

def postprocess(num_particles=1, tol=1E-3):
    """
    Load the output file and estimate the entropy.
    """

    # Load output file
    output = my_loadtxt("output.txt")
    num_particles = np.loadtxt("run_data.txt")

    # Find the starts of the runs
    start_indices = np.nonzero(output[:,0] == 1)[0]

    reps = len(start_indices)
    counts = []
    last_run_size = None
    warn = False

    for i in range(0, reps):
        start_index = start_indices[i]
        if i < reps - 1:
            run = output[start_index : start_indices[i+1], :]
        else:
            run = output[start_index : , :]

        # Don't count the last rep if it's incomplete
        if last_run_size != None and run.shape[0] != last_run_size:
            break
        last_run_size = run.shape[0]

        count = (run[:,1] > tol).sum()
        counts.append((run[:,1] > tol).sum())
        if count == run.shape[0]:
            warn = True

        print("Processed run {k}/{N}.".format(k=i+1, N=reps))

    # Convert to numpy array
    counts = np.array(counts)

    if len(counts) < reps:
        print("Skipping run {reps} as it is incomplete.".format(reps=reps))

    print("mean(depth) = {mc}.".format(mc=counts.mean() / num_particles))
    print("std (depth) = {sd}.".format(sd=counts.std()  / num_particles))
    print("sem (depth) = {sem}.".format(\
            sem=counts.std() / num_particles/np.sqrt(len(counts))))

    if warn:
        print("WARNING: A run didn't achieve the desired tolerance.")

#    # Estimate of the differential entropy
#    # log(probability) ~= log(2 * tol * density)
#    H = counts.mean() / num_particles + np.log(2 * tol)
#    stderr = (counts.std() / num_particles) / np.sqrt(reps)
#    print("H = {H} +- {stderr} nats.".format(H=H, stderr=stderr))
#    plt.hist(mean_depth, 100, color=[0.2, 0.2, 0.2])
#    plt.show()


def my_loadtxt(filename, single_precision=False):
    """
    Load rectangular arrays from a file, faster than numpy.loadtxt does it.
    """
    # Open the file
    f = open(filename, "r")

    # Storage
    results = []

    # Rows and columns
    nrow = 0
    ncol = None

    while(True):
        # Read the line and split by whitespace
        line = f.readline()
        cells = line.split()

        # Quit when you see a different number of columns
        if ncol is not None and len(cells) != ncol:
            break

        # Non-comment lines
        if cells[0][0] != "#":
            # If it's the first one, get the number of columns
            if ncol is None:
                ncol = len(cells)

            # Otherwise, include in results
            if single_precision:
                results.append(np.array([float(cell) for cell in cells],\
                                                          dtype="float32"))
            else:
                results.append(np.array([float(cell) for cell in cells]))
            nrow += 1

    results = np.vstack(results)
    return results


if __name__ == "__main__":
    postprocess()

