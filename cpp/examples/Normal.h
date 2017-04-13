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
*
* Differential entropies (true, analytic), in nats.
* H(mu)     =   3.72152
* H(x | mu) =   141.894
* H(mu, x)  =   145.615
* H(mu | x) = -0.883697
* H(x)      =   146.499
* I[x, y]   =   4.60522
*
* Numerical results (based on prob of meeting tolerance, not density!)
* H'(mu)    = 9.976  +- 0.032
* H'(x)     = 928.58 +- 0.47
* H'(mu, x) = 934.96 +- 0.80
* log(volume of n-ball) = n/2 * log(pi) - gammaln(n/2 + 1) + n*log(tol)
*   For mu, it's -6.21461. For x, it's -782.017.
* 
*/

class Normal
{
    private:
        // An unknown parameter
        double mu;

        // Data
        std::vector<double> xs;

        // Log-likelihood (useful for one of the proposals)
        double logl;
        void calculate_logl();

        // Helper for perturb
        double perturb_mu(RNG& rng);

    public:
        // Do-nothing constructor
        Normal();

        // Generate from the distribution
        void generate(RNG& rng);

        // Metropolis proposal
        double perturb(RNG& rng);

        // Printing to stream
        void print(std::ostream& out) const;

        // Available distance functions
        static double parameter_distance(const Normal& normal1,
                                         const Normal& normal2);
        static double data_distance(const Normal& normal1,
                                    const Normal& normal2);
        static double joint_distance(const Normal& normal1,
                                     const Normal& normal2);

};

} // namespace InfoNest

#endif

