// Includes
#include <iostream>
#include <ctime>
#include "Rep.hpp"
#include "RNG.h"
#include "Examples/Normal.h"

void copyright();

int main()
{
    // Create random number generator, seeded with time
    InfoNest::RNG rng(time(0));

    // Total number of reps to do
    int num_reps = 10000;

    // Clear output file
    std::fstream fout("output.txt", std::ios::out);
    fout.close();

    for(int i=0; i<num_reps; ++i)
    {
        // Create a Rep.
        InfoNest::Rep<InfoNest::Normal> rep(10, 1000, 25.0, rng);

        // Initialise and execute it.
        rep.initialise();
        rep.execute();

        std::cout<<"Completed "<<(i+1)<<" reps."<<std::endl;
    }

    return 0;
}

