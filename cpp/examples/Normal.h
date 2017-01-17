#ifndef InfoNest_Normal
#define InfoNest_Normal

// Includes
#include <ostream>
#include "../RNG.h"

namespace InfoNest
{

/*
* A 2D gaussian example.
* p(mu)     ~ N(0, 10^2)
* p(x | mu) ~ N(x, 1),    and there are 100 of them
*/

class Normal
{
    private:
        // The two actual quantities
        double mu;
        std::vector<double> xs;

    public:
        // Do-nothing constructor
        Normal();

        // Generate from the distribution
        void generate(RNG& rng);

        // Metropolis proposal
        double perturb(RNG& rng);

        // Printing to stream
        void print(std::ostream& out);

        // Distance function
        static double distance(const Normal& normal1,
                               const Normal& normal2);
};

} // namespace InfoNest

#endif

