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
* of the algorithm. The template parameter `Particle`
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

        // A reference to the RNG being used
        RNG& rng;

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
        void iterate(bool generate_replacement);

        // Replace the specified particle.
        void replace(int which);

        // Forbid copying and copy-construction.
        Rep(const Rep& other);
        Rep& operator = (const Rep& other);

    public:        
        // Constructor. You need to pass in the NS parameters
        // and an RNG.
        Rep(size_t num_particles, size_t mcmc_steps, double depth, RNG& rng);

        // Destructor. Closes output file.
        ~Rep();

        // Initialise all the particles
        void initialise();

        // Execute the Rep.
        void execute();
};


/******************************/
/* IMPLEMENTATION STARTS HERE */
/******************************/

template<class Particle>
Rep<Particle>::Rep(size_t num_particles, size_t mcmc_steps, double depth,
                   RNG& rng)
:num_particles(num_particles)
,mcmc_steps(mcmc_steps)
,depth(depth)
,rng(rng)
,particles(num_particles)
,distances(num_particles)
,iteration(0)
{
    // Check that the input was at least partially sane
    if(num_particles < 1 || mcmc_steps < 1 || depth < 1.0)
        throw std::domain_error("Bad input to Rep constructor.");

    // Open the output file in append mode.
    fout.open("output.txt", std::ios::out | std::ios::app);
}


template<class Particle>
Rep<Particle>::~Rep()
{
    fout.close();
}



template<class Particle>
void Rep<Particle>::initialise()
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
    // Evaluate the distance of each particle from the reference particle.
    for(size_t i=0; i<num_particles; ++i)
        distances[i] = Particle::distance(reference_particle, particles[i]);
}

template<class Particle>
void Rep<Particle>::execute()
{
    // Just do the required number of NS iterations
    // to reach `depth`.
    int steps = static_cast<int>(num_particles * depth);
    for(int i=0; i<steps; ++i)
        iterate(i != steps-1);
}

template<class Particle>
void Rep<Particle>::iterate(bool generate_replacement)
{
    // Find the worst particle.
    int worst = 0;
    for(size_t i=1; i<num_particles; ++i)
        if(distances[i] > distances[worst])
            worst = i;

    // Write its information to the output file.
    fout << iteration << ' ' << distances[worst] << std::endl;

    // Generate replacement (unless told not to!)
    if(generate_replacement)
        replace(worst);

    // Increment the iteration counter.
    ++iteration;
}

template<class Particle>
void Rep<Particle>::replace(int which)
{
    // Distance threshold
    double threshold = distances[which];

    // If num_particles > 1, clone a survivor.
    if(num_particles > 1)
    {
        int copy;
        do
        {
            copy = rng.rand_int(num_particles);
        }while(copy == which);

        particles[which] = particles[copy];
        distances[which] = distances[copy];
    }

    // Stuff needed for MCMC
    Particle proposal;
    double proposal_distance;
    double logA, alpha;

    // Do the MCMC
    for(size_t i=0; i<mcmc_steps; ++i)
    {
        // Do proposal
        proposal = particles[which];
        logA = proposal.perturb(rng);
        proposal_distance = Particle::distance(reference_particle, proposal);

        // Acceptance probability
        alpha = (logA >= 0.0) ? (1.0) : (exp(logA));

        // Check for acceptance
        if(proposal_distance < threshold && rng.rand() <= alpha)
        {
            particles[which] = proposal;
            distances[which] = proposal_distance;
        }
    }
}


} // namespace InfoNest

#endif

