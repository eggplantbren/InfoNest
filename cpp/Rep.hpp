#ifndef InfoNest_Rep
#define InfoNest_Rep

// Includes
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "RNG.h"
#include "Utils.h"

namespace InfoNest
{


// Two modes - standard and 'conditional entropy mode'
enum class Mode { standard, conditional_entropy };

// A convenient name for a distance function
// This is like a "template typedef"
template <class Particle>
using DistFunc = std::function<double(const Particle&, const Particle&)>;


/*
* An object of this class represents a single `rep`
* of the algorithm. The template parameter `Particle`
* determines what problem you're working on.
*/
template<class Particle>
class Rep
{
    private:
        // Distance function to use
        DistFunc<Particle> dist;

        // Identity
        const size_t id;

        // Nested Sampling parameters
        const size_t num_particles;
        const size_t mcmc_steps;
        const double depth;

        // MCMC steps for conditional entropy mode
        const size_t mcmc_ce_mode;

        // A reference to the RNG being used
        RNG& rng;

        // The reference particle
        Particle reference_particle;

        // The other particles
        std::vector<Particle> particles;

        // Distances
        std::vector<double> distances;

        // Tiebreakers for distances
        std::vector<double> tiebreakers;

        // Iteration counter
        unsigned int iteration;

        // Output file handle, and some cache
        std::fstream fout;
        std::vector<std::tuple<unsigned int, double>> cache;

        // Member function to compute all the distances
        void compute_distances();

        // Do a single NS iteration
        void iterate(bool last);

        // Replace the specified particle.
        void replace(int which);

        // Forbid copying and copy-construction.
        Rep(const Rep& other);
        Rep& operator = (const Rep& other);

    public:        
        // Constructor. You need to pass in the NS parameters
        // and an RNG.
        Rep(size_t id,
            size_t num_particles,
            size_t mcmc_steps,
            double depth,
            RNG& rng,
            const DistFunc<Particle>& dist,
            size_t _mcmc_ce_mode=10000);

        // Destructor. Closes output file.
        ~Rep();

        // Initialise all the particles
        void initialise(RNG& temp_rng, Mode mode);

        // Execute the Rep.
        void execute();
};


/******************************/
/* IMPLEMENTATION STARTS HERE */
/******************************/

template<class Particle>
Rep<Particle>::Rep(size_t id,
                   size_t num_particles, size_t mcmc_steps, double depth,
                   RNG& rng,
                   const DistFunc<Particle>& dist,
                   size_t _mcmc_ce_mode)
:dist(dist)
,id(id)
,num_particles(num_particles)
,mcmc_steps(mcmc_steps)
,depth(depth)
,mcmc_ce_mode(_mcmc_ce_mode)
,rng(rng)
,particles(num_particles)
,distances(num_particles)
,tiebreakers(num_particles)
,iteration(0)
{
    // Check that the input was at least partially sane
    if(num_particles < 1 || mcmc_steps < 1 || depth < 1.0)
        throw std::invalid_argument("Bad input to Rep constructor.");

    // Open the output file in append mode.
    fout.open("output.txt", std::ios::out | std::ios::app);
}


template<class Particle>
Rep<Particle>::~Rep()
{
    fout.close();
}



template<class Particle>
void Rep<Particle>::initialise(RNG& temp_rng, Mode mode)
{
    // Generate the reference particle from the distribution
    reference_particle.generate(temp_rng);

    // Generate the other particles
    for(size_t k=0; k<particles.size(); ++k)
    {
        if(mode == Mode::standard)
            particles[k].generate(rng);
        else if(mode == Mode::conditional_entropy)
        {
            if(k == 0)
                particles[k] = reference_particle;
            else
                particles[k] = particles[k-1];

            // Do Metropolis
            std::cout << "Generating initial particle..." << std::flush;
            for(size_t i=0; i<mcmc_ce_mode; ++i)
            {
                auto proposal = particles[k];
                double logA = proposal.perturb(rng);

                if(rng.rand() <= exp(logA))
                    particles[k] = proposal;
            }
            std::cout << "done." << std::endl;
        }
    }

    // Compute all the distances
    compute_distances();

    // Assign tiebreakers
    for(double& tb: tiebreakers)
        tb = rng.rand();

    // Set iteration number
    iteration = 1;
}


template<class Particle>
void Rep<Particle>::compute_distances()
{
    // Evaluate the distance of each particle from the reference particle.
    for(size_t i=0; i<num_particles; ++i)
        distances[i] = dist(reference_particle, particles[i]);
}

template<class Particle>
void Rep<Particle>::execute()
{
    // Just do the required number of NS iterations
    // to reach `depth`.
    int steps = static_cast<int>(num_particles * depth);
    for(int i=0; i<steps; ++i)
        iterate(i == steps-1);
}

template<class Particle>
void Rep<Particle>::iterate(bool last)
{
    // Find the worst particle.
    int worst = 0;
    for(size_t i=1; i<num_particles; ++i)
        if(distances[i] > distances[worst] ||
            (distances[i] == distances[worst] &&
             tiebreakers[i] > tiebreakers[worst]))
            worst = i;

    // Write its information to the output file.
    cache.push_back(
        std::tuple<unsigned int, double>{iteration, distances[worst]}
                   );
    unsigned int it; double d;
    if(cache.size() >= 100 || last)
    {
        for(size_t i=0; i<cache.size(); ++i)
        {
            std::tie(it, d) = cache[i];
            fout << it << ' ' << d << '\n';
        }
        fout << std::flush;
        cache.clear();

        // Output the two particles
        std::fstream particles_file("latest_particles.txt", std::ios::out);
        reference_particle.print(particles_file);
        particles_file << '\n';
        particles[worst].print(particles_file);
        particles_file << '\n';
        particles_file.close();
    }

    // Generate replacement (unless told not to!)
    if(!last)
        replace(worst);

    // Increment the iteration counter.
    ++iteration;
}

template<class Particle>
void Rep<Particle>::replace(int which)
{
    // Distance threshold
    double threshold = distances[which];
    double tb_threshold = tiebreakers[which];

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
        tiebreakers[which] = tiebreakers[copy];
    }

    // Stuff needed for MCMC
    Particle proposal;
    double proposal_distance;
    double proposal_tb;
    double logA, alpha;

    // Count acceptances
    int accepts = 0;

    // Do the MCMC
    for(size_t i=0; i<mcmc_steps; ++i)
    {
        // Do proposal
        proposal = particles[which];
        logA = proposal.perturb(rng);
        proposal_distance = dist(reference_particle, proposal);
        proposal_tb = tiebreakers[which] + rng.randh();
        wrap(proposal_tb, 0.0, 1.0);

        // Acceptance probability
        alpha = (logA >= 0.0) ? (1.0) : (exp(logA));

        // Check for acceptance
        if(rng.rand() <= alpha)
        {
            if(proposal_distance < threshold ||
                (proposal_distance == threshold &&
                 proposal_tb < tb_threshold))
            {
                particles[which] = proposal;
                distances[which] = proposal_distance;
                tiebreakers[which] = proposal_tb;

                ++accepts;
            }
        }
    }

    if(iteration % particles.size() == 0)
    {
        std::cout<<"Rep "<<id<<", ";
        std::cout<<"iteration "<<iteration<<". ";
        std::cout<<"Acceptance fraction = "<<accepts<<" / "<<mcmc_steps<<".";
        std::cout<<std::endl;
    }
}


} // namespace InfoNest

#endif

