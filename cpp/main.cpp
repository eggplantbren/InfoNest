// Includes
#include <iostream>
#include <ctime>
#include "Rep.hpp"
#include "RNG.h"
#include "examples/Normal.h"

// A function that does a run
template<class ParticleType>
void execute(InfoNest::RNG& rng,
             double depth,
             size_t num_reps=1000,
             size_t num_particles=1,
             size_t mcmc_steps=1000);


// MAIN
int main()
{
    // Create random number generator, seeded with time
    InfoNest::RNG rng(time(0));

    // Do the runs.
    execute<InfoNest::Normal>(rng, 30.0);

    return 0;
}


// Implementation of execute
template<class ParticleType>
void execute(InfoNest::RNG& rng,
             double depth,
             size_t num_reps,
             size_t num_particles,
             size_t mcmc_steps)
{
    // Clear output file
    std::fstream fout("output.txt", std::ios::out);
    fout.close();

    // Write run data to a file
    fout.open("run_data.txt", std::ios::out);
    fout << num_particles << "\t# num_particles." << std::endl;
    fout.close();

    // Do the reps.
    for(size_t i=0; i<num_reps; ++i)
    {
        // Create a Rep.
        InfoNest::Rep<ParticleType> rep(i+1,
                                        num_particles,
                                        mcmc_steps,
                                        depth,
                                        rng);

        // Initialise and execute it.
        rep.initialise();
        rep.execute();

        // Print a message.
        std::cout << "Completed " << (i+1) << " reps." << std::endl;
    }
}

