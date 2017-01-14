#ifndef InfoNest_Rep
#define InfoNest_Rep

// Includes
#include <exception>
#include <fstream>
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

        // Output file handle
        std::fstream fout;

        // Member function to compute all the distances
        void compute_distances();

        // Do a single NS iteration
        void iterate();

    public:        
        // Constructor. You need to pass in the NS parameters
        Rep(size_t num_particles, size_t mcmc_steps, double depth);

        // Destructor. Closes output file.
        ~Rep();

        // Initialise all the particles
        void initialise(RNG& rng);

        // Execute the Rep.
        void execute();
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
    if(num_particles < 1 || mcmc_steps < 1 || depth < 1.0)
        throw std::domain_error("Bad input to Rep constructor.");

    fout.open("output.txt", std::ios::out | std::ios::app);
}


template<class Particle>
Rep<Particle>::~Rep()
{
    fout.close();
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

template<class Particle>
void Rep<Particle>::execute()
{
    int steps = static_cast<int>(num_particles * depth);
    for(int i=0; i<steps; ++i)
        iterate();
}

template<class Particle>
void Rep<Particle>::iterate()
{
    // Find the worst particle.
    int worst = 0;
    for(size_t i=1; i<num_particles; ++i)
        if(distances[i] > distances[worst])
            worst = i;

    // Write its information to the output file.
    fout << iteration << ' ' << distances[worst] << std::endl;
}

} // namespace InfoNest

#endif

