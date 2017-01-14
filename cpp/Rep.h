#ifndef InfoNest_Rep
#define InfoNest_Rep

// Includes
#include <stdlib.h>
#include <vector>
#include "RNG.h"

namespace InfoNest
{

/*
* An object of this class represents a single `rep`
* of the algorithm. The template parameter `particle`
* determines what problem you're working on.
*/
template<class Particle>
class Rep
{
    private:
        // Nested Sampling parameters
        const size_t num_particles;
        const size_t mcmc_steps;
        const double depth;

        // The reference particle
        Particle reference_particle;

        // The other particles
        std::vector<Particle> particles;

        // Distances
        std::vector<double> distances;

        // Iteration counter
        unsigned int iteration;

        // Member function to compute all the distances
        void compute_distances();

    public:        
        // Constructor. You need to pass in the NS parameters
        Rep(size_t num_particles, size_t mcmc_steps, double depth);

        // Initialise all the particles
        void initialise(RNG& rng);

};


/******************************/
/* IMPLEMENTATION STARTS HERE */
/******************************/

template<class Particle>
Rep<Particle>::Rep(size_t num_particles, size_t mcmc_steps, double depth)
:num_particles(num_particles)
,mcmc_steps(mcmc_steps)
,depth(depth)
,particles(num_particles)
,distances(num_particles)
,iteration(0)
{

}


template<class Particle>
void Rep<Particle>::initialise(RNG& rng)
{
    // Generate the reference particle from the distribution
    reference_particle.generate(rng);

    // Generate the other particles
    for(auto& p: particles)
        p.generate(rng);

    // Compute all the distances
    compute_distances();

    // Set iteration number
    iteration = 1;
}


template<class Particle>
void Rep<Particle>::compute_distances()
{
    for(size_t i=0; i<num_particles; ++i)
        distances[i] = Particle::distance(reference_particle, particles[i]);
}

} // namespace InfoNest

#endif

