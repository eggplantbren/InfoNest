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
    x = 3*rng.randn(num)
    y = 0.1*x*np.abs(x) + 2*rng.randn(num)
    return {"x": x, "y": y}


# Set up a grid
x = np.linspace(-10.0, 10.0, 1001)
[x, y] = np.meshgrid(x, x)
y = y[::-1]

# Target density
p = density(x, y)
p /= p.sum()


# First image
plt.figure(figsize=(8, 7))
plt.imshow(p, extent=[x.min(), x.max(), y.min(), y.max()],
           cmap="Blues")
plt.xlabel(r"$x$")
plt.ylabel(r"$y$")
plt.title("A joint pdf $f(x, y)$")
plt.savefig("movie/image1.png", bbox_inches="tight")
print("Saved movie/image1.png")


# Second image
point = generate()
xref, yref = point["x"], point["y"]
plt.plot(xref, yref, "o", color="r", alpha=0.5, markersize=12)
plt.title("A reference point drawn from $f(x,y)$")
plt.savefig("movie/image2.png", bbox_inches="tight")
print("Saved movie/image2.png")


# Add contours of the distance function
dist = np.sqrt((x - xref)**2 + (y - yref)**2)
plt.contour(x, y, dist, 50, alpha=0.2, colors="red", linestyles="--")
plt.title("Distance from the reference point")
plt.savefig("movie/image3.png", bbox_inches="tight")
print("Saved movie/image3.png")


# Add Nested Sampling points
ns_points = generate(10)
plt.plot(ns_points["x"], ns_points["y"], "*", color="green",
         markersize=10, alpha=0.8)
plt.title("Nested Sampling")
plt.savefig("movie/image4.png", bbox_inches="tight")
print("Saved movie/image4.png")

# NS loop
for i in range(0, 60):

    # Find worst point
    dist = np.sqrt((ns_points["x"] - xref)**2 + (ns_points["y"] - yref)**2)
    worst = np.nonzero(dist == np.max(dist))[0]
    plt.plot(ns_points["x"][worst], ns_points["y"][worst], "^", color="black",
             markersize=10, alpha=0.8)
    plt.savefig("movie/image{k}.png".format(k=i+5), bbox_inches="tight")
    print("Saved movie/image{k}.png".format(k=i+5))

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
            break

