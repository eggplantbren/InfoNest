// Includes
#include <iostream>
#include <ctime>
#include "Rep.hpp"
#include "RNG.h"
#include "examples/Normal.h"

int main()
{
    // Run parameters
    size_t num_particles = 1;
    size_t mcmc_steps = 1000;
    double depth = 25.0;

    // Create random number generator, seeded with time
    InfoNest::RNG rng(time(0));

    // Total number of reps to do
    int num_reps = 10000;

    // Clear output file
    std::fstream fout("output.txt", std::ios::out);
    fout.close();

    // Write run data to a file
    fout.open("run_data.txt", std::ios::out);
    fout << num_particles << "\t# num_particles." << std::endl;
    fout.close();

    for(int i=0; i<num_reps; ++i)
    {
        // Create a Rep.
        InfoNest::Rep<InfoNest::Normal> rep(i+1,
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

    return 0;
}

