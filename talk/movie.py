# Imports
import matplotlib.pyplot as plt
import numpy as np
import numpy.random as rng
import os

# Create a directory for output
os.system("mkdir movie")

# Set rng seed
rng.seed(0)

# Set fonts
plt.rc("font", size=20, family="Serif", serif="Computer Sans")
plt.rc("text", usetex=True)

def density(x, y):
    """
    A 2D joint density used for the demo.
    """
    fx   = 1.0/np.sqrt(3*2*np.pi)*np.exp(-0.5*x**2 / 3**2)
    fy_x = 1.0/np.sqrt(2*2*np.pi)*np.exp(-0.5*(y - 0.1*x*np.abs(x))**2 / 2**2)
    return fx * fy_x

def generate(num=1):
    """
    Generate from the above density.
    """

    xx = []
    yy = []

    for i in range(num):
        while True:
            x = 3*rng.randn()
            y = 0.1*x*np.abs(x) + 2*rng.randn()
            if np.abs(x) < 10.0 and np.abs(y) < 10.0:
                break
        xx.append(x)
        yy.append(y)

    return {"x": np.array(xx), "y": np.array(yy)}


# Set up a grid
x = np.linspace(-10.0, 10.0, 1001)
[x, y] = np.meshgrid(x, x)
y = y[::-1]

# Target density
p = density(x, y)
p /= p.sum()

k = 1


for rep in range(10):

    # First image
    plt.clf()
    plt.figure(figsize=(8, 7))
    plt.imshow(np.sqrt(p), extent=[x.min(), x.max(), y.min(), y.max()],
               cmap="Blues")
    plt.xlabel(r"$x$")
    plt.ylabel(r"$y$")
    plt.title("A joint pdf $f(x, y)$")

    for j in range(20):
        plt.savefig("movie/image{k}.png".format(k="%0.6d" % k))
        print("Saved movie/image{k}.png".format(k="%0.6d" % k))
        k += 1

    # Second image
    point = generate()
    xref, yref = point["x"], point["y"]
    plt.plot(xref, yref, "o", color="r", alpha=0.5, markersize=12)
    plt.title("A reference point drawn from $f(x,y)$")

    for j in range(20):
        plt.savefig("movie/image{k}.png".format(k="%0.6d" % k))
        print("Saved movie/image{k}.png".format(k="%0.6d" % k))
        k += 1

    # Add contours of the distance function
    dist = np.sqrt((x - xref)**2 + (y - yref)**2)
    plt.contour(x, y, dist, levels=np.linspace(0.0, 40.0, 51),
                alpha=0.2, colors="red", linestyles="--")
    plt.title("Distance from the reference point")

    for j in range(20):
        plt.savefig("movie/image{k}.png".format(k="%0.6d" % k))
        print("Saved movie/image{k}.png".format(k="%0.6d" % k))
        k += 1

    # Add Nested Sampling points
    ns_points = generate(10)
    plt.plot(ns_points["x"], ns_points["y"], "*", color="green",
             markersize=10, alpha=0.8)
    plt.title("Nested Sampling")
    for j in range(20):
        plt.savefig("movie/image{k}.png".format(k="%0.6d" % k))
        print("Saved movie/image{k}.png".format(k="%0.6d" % k))
        k += 1

    # NS loop
    for i in range(0, 50):

        # Find worst point
        dist = np.sqrt((ns_points["x"] - xref)**2 + (ns_points["y"] - yref)**2)
        worst = np.nonzero(dist == np.max(dist))[0]
        plt.plot(ns_points["x"][worst], ns_points["y"][worst], "^", color="black",
                 markersize=10, alpha=0.8)
        plt.savefig("movie/image{k}.png".format(k="%0.6d" % k))
        print("Saved movie/image{k}.png".format(k="%0.6d" % k))

        # Replace worst point
        threshold = dist[worst]
        while True:
            new = generate()
            dist_new = np.sqrt((new["x"] - xref)**2 + (new["y"] - yref)**2)
            if dist_new < threshold:
                ns_points["x"][worst] = new["x"]
                ns_points["y"][worst] = new["y"]

                plt.plot(new["x"], new["y"], "*", color="green",
                            markersize=10, alpha=0.8)
                k += 1
                break


