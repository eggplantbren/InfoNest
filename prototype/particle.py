# Stuff accessible from outside (?)
__all__ = ["Particle"]

# Imports
import numpy as np
import numpy.random as rng
from utils import randh, wrap

class Particle:
    """
    An object of this class represents a point in the relevant
    hypothesis space.
    """

    def __init__(self):
        """
        Constructor does nothing.
        """
        pass


    def generate(self):
        """
        Generate the point from its probability distribution.
        """
        self.x = -10.0 + 20.0 * rng.rand()


    def perturb(self):
        """
        Metropolis proposal that would explore the probability distribution.
        Return ln(a) such that acceptance probability alpha = min(1, a).
        """
        self.x += randh()
        self.x = wrap(self.x, -10.0, 10.0)
        return 0.0


    def __str__(self):
        """
        Represent as a string.
        """
        return str(self.x)


def distance(particle1, particle2):
    """
    Distance between two particles.
    """
    return np.abs(particle1.x - particle2.x)




if __name__ == "__main__":
    """
    A few tiny little tests.
    """

    particle = Particle()
    particle.generate()

    for i in range(0, 1000):
        particle.perturb()
        print(particle)

