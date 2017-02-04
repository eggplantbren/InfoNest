// Includes
#include <iostream>
#include <ctime>
#include "Execute.hpp"
#include "RNG.h"
#include "examples/AllExamples.h"

// MAIN
int main()
{
    // What example are we using? The class and the distance function.
    typedef InfoNest::Normal TheExample;
    const auto& dist_func = TheExample::parameter_distance;

    // Create random number generators, seeded with time
    unsigned long seed1 = time(0);
    unsigned long seed2 = seed1 + 1;
    InfoNest::RNG rng1(seed1);
    InfoNest::RNG rng2(seed2);

    // Define run parameters
    constexpr double depth         = 30.0;
    constexpr size_t num_reps      = 1000;
    constexpr size_t num_particles = 100;
    constexpr size_t mcmc_steps    = 1000;

    // Do the run.
    InfoNest::execute<TheExample>(rng1, rng2, depth, num_reps, num_particles,
                                  mcmc_steps, dist_func);

    return 0;
}

