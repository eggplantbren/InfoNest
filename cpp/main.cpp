// Includes
#include <iostream>
#include <ctime>
#include "Rep.hpp"
#include "RNG.h"
#include "Examples/Normal.h"

int main()
{
    // Create random number generator, seeded with time
    InfoNest::RNG rng(time(0));

    // Create a Rep.
    InfoNest::Rep<InfoNest::Normal> rep(10, 1000, 25.0);

    return 0;
}

