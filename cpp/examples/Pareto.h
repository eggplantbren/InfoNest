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
        // Lower limit and slope
        // of pareto distribution for scores
        double x_min;
        double alpha;

        // Number of games played (gets rounded down to
        // an integer when needed)
        double N;

        // Top ten scores
        std::vector<unsigned int> top_ten;

        // Function that generates the top ten scores
        void generate_data(RNG& rng);

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

