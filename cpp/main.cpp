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

    // Create random number generator, seeded with time
    InfoNest::RNG rng(time(0));

    // Define run parameters
    constexpr double depth         = 30.0;
    constexpr size_t num_reps      = 1000;
    constexpr size_t num_particles = 1;
    constexpr size_t mcmc_steps    = 1000;

    // Do the run.
    InfoNest::execute<TheExample>(rng, depth, num_reps, num_particles,
                                  mcmc_steps, dist_func);

    return 0;
}

