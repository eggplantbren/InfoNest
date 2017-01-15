#ifndef InfoNest_Sinusoid
#define InfoNest_Sinusoid

// Includes
#include <ostream>
#include <vector>
#include "../RNG.h"

namespace InfoNest
{

/*
* A sinusoid example - starting to get a bit more serious in size
* and complexity, but still simple.
*/
class Sinusoid
{
    private:
        // Number of data points and so on
        static constexpr size_t N = 101;
        static constexpr double t_min = 0.0;
        static constexpr double t_max = 1.0;
        static constexpr double t_range = t_max - t_min;
        static constexpr double dt = t_range / (N - 1);

        // Noise sd
        static constexpr double sigma = 1.0;

    private:
        // Amplitude, period, and phase
        double A, T, phi;

        // Latent coordinates for noise
        std::vector<double> n;

        // The data
        std::vector<double> y;

        // Compute y
        void assemble();

    public:
        // Do-nothing constructor
        Sinusoid();

        // Generate from the distribution
        void generate(RNG& rng);

        // Metropolis proposal
        double perturb(RNG& rng);

        // Printing to stream
        void print(std::ostream& out);

        // Distance function
        static double distance(const Sinusoid& s1,
                               const Sinusoid& s2);
};

} // namespace InfoNest

#endif

