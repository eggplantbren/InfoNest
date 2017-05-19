#ifndef InfoNest_Pareto
#define InfoNest_Pareto

// Includes
#include <ostream>
#include <tuple>
#include "../RNG.h"

namespace InfoNest
{

class Pareto
{
    private:
        static constexpr double x_min = 1.0;

        // Pareto slope
        double alpha;

        // The data
        static constexpr size_t N = 100;
        std::vector<double> us;
        std::vector<double> xs;

        // Function that generates the top ten scores
        void generate_data();

        // Sum of first half of data
        double sum_first_half() const;

        // Sum of second half of data
        double sum_second_half() const;

    public:
        // Do-nothing constructor
        Pareto();

        // Generate from the distribution
        void generate(RNG& rng);

        // Metropolis proposal
        double perturb(RNG& rng);

        // Printing to stream
        void print(std::ostream& out) const;

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

