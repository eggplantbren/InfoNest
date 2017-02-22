#ifndef InfoNest_GroupDiff
#define InfoNest_GroupDiff

// Includes
#include <ostream>
#include "../RNG.h"

namespace InfoNest
{

class GroupDiff
{
    private:
        // Unknown parameters
        double mu1, sigma1;
        double diff_mu;         // (mu2 - mu1) / sigma1
        double diff_log_sigma;

        // Derived parameters
        double mu2, sigma2;

        // Data
        std::vector<double> xs1;
        std::vector<double> xs2;

        // Log-likelihood (useful for one of the proposals)
        double logl;
        void calculate_logl();
        void compute_derived_quantities();

        // Helper for perturb
        double perturb_parameter(RNG& rng);

    public:
        // Do-nothing constructor
        GroupDiff();

        // Generate from the distribution
        void generate(RNG& rng);

        // Metropolis proposal
        double perturb(RNG& rng);

        // Printing to stream
        void print(std::ostream& out);

        // Available distance functions
        static double parameter_distance(const GroupDiff& GroupDiff1,
                                         const GroupDiff& GroupDiff2);
        static double data_distance(const GroupDiff& GroupDiff1,
                                    const GroupDiff& GroupDiff2);
        static double joint_distance(const GroupDiff& GroupDiff1,
                                     const GroupDiff& GroupDiff2);

};

} // namespace InfoNest

#endif

