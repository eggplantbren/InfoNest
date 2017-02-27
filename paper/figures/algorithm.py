from plot_utils import rc
import numpy as np
import numpy.random as rng
import matplotlib.pyplot as plt

# Set default fonts etc
rc()

"""
Generate some figures and perhaps a movie.
"""

def density(x, y):
    """
    A 2D joint density used for the demo.
    """
    fx   = 1.0/np.sqrt(3*2*np.pi)*np.exp(-0.5*x**2 / 3 **2)
    fy_x = 1.0/np.sqrt(2*2*np.pi)*np.exp(-0.5*(y - 0.1*x*np.abs(x))**2 / 2**2)
    return fx * fy_x

def generate(num=1):
    """
    Generate from the above density.
    """
    x = 3*rng.randn(num)
    y = 0.1*x*np.abs(x) + 2*rng.randn(num)
    return {"x": x, "y": y}


if __name__ == "__main__":
    # RNG seed
    rng.seed(0)

    # A grid
    x = np.linspace(-10.0, 10.0, 1001)
    y = np.linspace(-10.0, 10.0, 1001)
    [x, y] = np.meshgrid(x, y[::-1])

    # Evaluate the pdf
    f = density(x, y)

    # Generate from the pdf
    samples = generate(1)

    # Euclidean distance from the sample
    distance = np.sqrt((x - samples["x"][0])**2 + \
                       (y - samples["y"][0])**2)

    # Plot the pdf and the samples
    plt.imshow(density(x, y), cmap="Blues", interpolation="nearest",
               extent=[x.min(), x.max(), y.min(), y.max()])
    plt.xlabel("$x$", fontsize=16)
    plt.ylabel("$y$", fontsize=16)
    plt.contour(x, y, density(x, y), 20, colors=["b"], alpha=0.1)
    plt.plot(samples["x"], samples["y"], "r.", alpha=0.5, markersize=20)
    plt.contour(x, y, distance, 20, colors=["r"], alpha=0.1)
    plt.axis([x.min(), x.max(), y.min(), y.max()])
    plt.savefig("algorithm.pdf", bbox_inches="tight")
    plt.show()

