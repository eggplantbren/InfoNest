// Includes
#include <iostream>
#include <ctime>
#include "Execute.hpp"
#include "RNG.h"
#include "examples/Normal.h"

// MAIN
int main()
{
    // Create random number generator, seeded with time
    InfoNest::RNG rng(time(0));

    // Do the runs.
    InfoNest::execute<InfoNest::Normal>(rng, 30.0, 1000, 1, 1000);

    return 0;
}


