#include <iostream>
#include <fstream>
#include "../examples/Sinusoid.h"

int main()
{
    // Make an RNG
    InfoNest::RNG rng(time(0));

    // Make and initialise a Sinusoid
    InfoNest::Sinusoid s;
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
            std::cout << "Done " << (i+1) << " steps." << std::endl;
        }
    }

    fout.close();

    return 0;
}

