#ifndef InfoNest_Normal
#define InfoNest_Normal

// Includes
#include <ostream>
#include "../RNG.h"

namespace InfoNest
{

/*
* A 2D gaussian example.
* p(x)     ~ N(0, 10^2)
* p(y | x) ~ N(x, 1)
* ==>
* p(y)     ~ N(0, sd=sqrt(101))

* H[p(x)]    = 3.72152363.            (differential entropy)
* H[p(y)]    = 3.72649879.            (differential entropy)
* H[p(x, y)] = 5.14046216.            (differential entropy)
* I[x; y]    = 2.3075603.             (mutual information)
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

        // Printing to stream
        void print(std::ostream& out);

        // Distance function
        static double distance(const Normal& normal1, const Normal& normal2);
};

} // namespace InfoNest

#endif

