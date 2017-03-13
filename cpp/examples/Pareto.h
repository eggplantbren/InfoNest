#ifndef InfoNest_Pareto
#define InfoNest_Pareto

// Includes
#include <ostream>
#include "../RNG.h"

namespace InfoNest
{

class Pareto
{
    private:
        // Lower limit
        static constexpr double x_min = 1.0;

        // Slope
        double alpha;

        // Data
        std::vector<double> xs;

    public:
        // Do-nothing constructor
        Pareto();

        // Generate from the distribution
        void generate(RNG& rng);

        // Metropolis proposal
        double perturb(RNG& rng);

        // Printing to stream
        void print(std::ostream& out);

        // Available distance functions
        static double parameter_distance(const Pareto& pareto1,
                                         const Pareto& pareto2);
        static double data_distance(const Pareto& pareto1,
                                    const Pareto& pareto2);
        static double joint_distance(const Pareto& pareto1,
                                     const Pareto& pareto2);

};

} // namespace InfoNest

#endif

