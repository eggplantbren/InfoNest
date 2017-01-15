#ifndef InfoNest_SamplingTest
#define InfoNest_SamplingTest

// Includes
#include <iostream>
#include <fstream>
#include <cmath>
#include "RNG.h"

// Do MCMC sampling and iid sampling on a given example.
template<class Example>
void sampling_test()
{
    // Make an RNG
    InfoNest::RNG rng(time(0));

    // Make and initialise a Sinusoid
    Example s;
    s.generate(rng);

    // Output file
    std::fstream fout("output.txt", std::ios::out);

    // Do Metropolis steps
    int steps = 1000000;
    int thin = 100;

    for(int i=0; i<steps; ++i)
    {
        auto proposal = s;
        double logH = proposal.perturb(rng);

        if(rng.rand() <= exp(logH))
            s = proposal;

        if((i+1) % thin == 0)
        {
            s.print(fout);
            fout << std::endl;
            std::cout << "Done " << (i+1) << " MCMC steps." << std::endl;
        }
    }

    // Now do independent generating
    steps /= thin;
    for(int i=0; i<steps; ++i)
    {
        s.generate(rng);

        s.print(fout);
        fout << std::endl;
        std::cout << "Done " << (i+1) << " generates." << std::endl;
    }

    fout.close();

} // void sampling_test()

#endif

