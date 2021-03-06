#ifndef InfoNest_Execute
#define InfoNest_Execute

// Includes
#include "Rep.hpp"
#include "RNG.h"

namespace InfoNest
{

// A convenient name for a distance function
template <class Particle>
using DistFunc = std::function<double(const Particle&, const Particle&)>;

// A function that does a run
template<class Particle>
void execute(RNG& rng0,
             RNG& rng1,
             double depth,
             size_t num_reps,
             size_t num_particles,
             size_t mcmc_steps,
             const DistFunc<Particle>& dist_func,
             Mode mode = Mode::standard,
             size_t _mcmc_ce_mode=100000); // Number of MCMC steps to
                                           // generate reference particles
                                           // in conditional entropy mode.


/* Implementations below */

// Implementation of execute
template<class Particle>
void execute(RNG& rng0,
             RNG& rng1,
             double depth,
             size_t num_reps,
             size_t num_particles,
             size_t mcmc_steps,
             const DistFunc<Particle>& dist_func,
             Mode mode,
             size_t _mcmc_ce_mode)
{
    // Clear output files
    std::fstream fout("output.txt", std::ios::out);
    fout.close();
    fout.open("reference_particles.txt", std::ios::out);
    fout.close();

    // Write run data to a file
    fout.open("run_data.txt", std::ios::out);
    fout << num_particles << "\t# num_particles." << std::endl;
    fout.close();

    // Do the reps.
    for(size_t i=0; i<num_reps; ++i)
    {
        // Create a Rep.
        InfoNest::Rep<Particle> rep(i+1,
                                        num_particles,
                                        mcmc_steps,
                                        depth,
                                        rng1,
                                        dist_func,
                                        _mcmc_ce_mode);

        // Initialise and execute it.
        rep.initialise(rng0, mode);
        rep.execute();

        // Print a message.
        std::cout << "Completed " << (i+1) << " reps." << std::endl;
    }
}

} // namespace InfoNest

#endif

