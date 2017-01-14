# Stuff accessible from outside (?)
__all__ = ["Run"]

# Imports
from particle import Particle, distance




class Run:
    """
    An object of this class represents a single run of the algorithm.
    Ultimately, you'll need to do many runs and average the results.
    """

    def __init__(self, num_particles=1, mcmc_steps=1000):
        """
        Constructor. Pass in the desired number of NS particles
        and the number of mcmc steps per iteration.
        """
        self.num_particles = num_particles
        self.mcmc_steps = mcmc_steps

    def initialise(self):
        """
        Generate the reference particle and the comparison particles.
        """

        self.reference_particle = Particle.factory()
        self.comparison_particles = [ Particle.factory()
                                        for i in range(0, self.num_particles) ]
        self.compute_distances()


    def compute_distances(self):
        """
        Compute the distance from the reference particle to each
        comparison particle.
        """
        self.distances = [ distance(self.reference_particle, p)
                                for p in self.comparison_particles ]

    



if __name__ == "__main__":
    """
    Little test.
    """

    run = Run(10)
    run.initialise()

    print(run.distances)

