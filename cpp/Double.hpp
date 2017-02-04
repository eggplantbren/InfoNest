#ifndef InfoNest_Double
#define InfoNest_Double

#include "DNest4/code/RNG.h"
#include <tuple>

namespace InfoNest
{

// For mutual information purposes, this allows a
// model which contains two copies of a standard model.

template<class Particle>
class Double
{
    private:
        // The two particles
        std::tuple<Particle, Particle> particles;

    public:
        // Do-nothing constructor
        Double();

        // Generate the two particles
        void generate(RNG& rng);

        
}

} // namespace InfoNest

#endif

