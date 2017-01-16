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

/*
* Numerical results:
* NB: these algorithm computes log-probabilities of meeting the tolerance,
*     so a correction factor of log(2E-3) (log-volume)
*     has been applied to make them match the above by converting probability
*     to density. For H[p(x,y)], the correction factor is 2*log(2E-3).
*  H[p(x)]   = 3.761 +- 0.040
*  H[p(y)]   = 3.776 +- 0.048
*  H[p(x,y)] = 5.167 +- 0.055
*  I[x; y]   = 2.370 +- 0.083 (computed from the above).
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

