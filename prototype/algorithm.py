# Stuff accessible from outside (?)
__all__ = ["Run"]

# Imports
from copy import deepcopy
import numpy as np
import numpy.random as rng
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
        self.particles = [ Particle.factory()
                                        for i in range(0, self.num_particles) ]
        self.compute_distances()
        self.iteration = 1


    def compute_distances(self):
        """
        Compute the distance from the reference particle to each
        comparison particle.
        """
        self.distances = [ distance(self.reference_particle, p)
                                for p in self.particles ]
        self.distances = np.array(self.distances)

    def iterate(self):
        """
        Find the worst particle and replace it, NS style.
        """

        # Index of worst particle
        worst = np.argmax(self.distances)

        # Print iteration, and worst particle's distance
        print( str(self.iteration) + " " + str(self.distances[worst]) )

        # Replace worst particle.
        self.replace(worst)

        # Update iteration counter.
        self.iteration += 1


    def replace(self, worst):
        """
        Replace the worst particle.
        """
        # Threshold
        threshold = self.distances[worst]

        # Copy a survivor
        if self.num_particles > 1:
            while True:
                copy = rng.randint(self.num_particles)
                if copy != worst:
                    break
            self.particles[worst] = deepcopy(self.particles[copy])

        # Do mcmc
        for i in range(0, self.mcmc_steps):
            # Generate proposal
            proposal = deepcopy(self.particles[worst])
            logA = proposal.perturb()
            proposal_distance = distance(self.reference_particle, proposal)
            if logA > 0.0:
                logA = 0.0

            # Acceptance
            if proposal_distance < threshold and rng.rand() <= np.exp(logA):
                self.particles[worst] = proposal
                self.distances[worst] = proposal_distance

if __name__ == "__main__":
    """
    Little test.
    """

    # Number of repetitions
    reps = 100

    for rep in range(0, reps):
        run = Run(1)
        run.initialise()

        for i in range(0, 20):
            run.iterate()



