import numpy as np
import numpy.random as rng
import matplotlib.pyplot as plt

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
    samples = generate(100)

    # Plot the pdf and the samples
    plt.imshow(density(x, y), cmap="viridis", interpolation="nearest",
               extent=[x.min(), x.max(), y.min(), y.max()])
    plt.xlabel("$x$", fontsize=20)
    plt.ylabel("$y$", fontsize=20)
    plt.hold(True)
    plt.plot(samples["x"], samples["y"], "w.", alpha=0.6, markersize=10)
    plt.axis([x.min(), x.max(), y.min(), y.max()])
    plt.savefig("figure.png")
    plt.show()

