#ifndef InfoNest_Normal
#define InfoNest_Normal

// Includes
#include <ostream>
#include "../RNG.h"

namespace InfoNest
{

/*
* A 2D gaussian example.
* p(x)     ~ N(0,1)
* p(y | x) ~ N(x,1)
* ==>
* p(y)     ~ N(0, sd=sqrt(2))
* H[p(y)] = 1.76551.
*/
class Normal
{
    private:
        // The two actual quantities
        double x;
        double y;

        // Latent coordinates
        double n1, n2;
        void assemble();

    public:
        // Do-nothing constructor
        Normal();

        // Generate from the distribution
        void generate(RNG& rng);

        // Metropolis proposal
        double perturb(RNG& rng);

        // Printing
        void print(std::ostream& out);
};

} // namespace InfoNest

#endif

