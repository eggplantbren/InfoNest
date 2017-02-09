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
        static constexpr double sigma = 0.1;

    private:
        // Amplitude, log-period, and phase
        double A, log10_period, phi;

        // Model curve
        std::vector<double> t;
        std::vector<double> mu;

        // The data
        std::vector<double> y;

        // Log likelihood, i.e. ln p(y | params).
        // Only needed for one of the proposals
        double logl;

        // Calculate log likelihood
        void calculate_logl();

        // Compute mu from the current parameter values
        void calculate_mu();

        // Helper for perturb
        double perturb_parameters(RNG& rng);

    public:
        // Do-nothing constructor
        Sinusoid();

        // Generate from the distribution
        void generate(RNG& rng);

        // Metropolis proposal
        double perturb(RNG& rng);

        // Printing to stream
        void print(std::ostream& out);

    public:
        // A few options to use for `distance`
        static double parameter_distance
                            (const Sinusoid& s1, const Sinusoid& s2);
        static double data_distance(const Sinusoid& s1, const Sinusoid& s2);
        static double joint_distance(const Sinusoid& s1, const Sinusoid& s2);
};

} // namespace InfoNest

#endif

