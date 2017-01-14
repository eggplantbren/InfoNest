#ifndef InfoNest_Rep
#define InfoNest_Rep

#include <stdlib.h>

namespace InfoNest
{

/*
* An object of this class represents a single `rep`
* of the algorithm. The template parameter `particle`
* determines what problem you're working on.
*/
template<class Particle>
class Rep
{
    private:
        // Nested Sampling parameters
        const size_t num_particles;
        const size_t mcmc_steps;
        const double depth;

    public:        
        // Constructor. You need to pass in the NS parameters
        Rep(size_t num_particles, size_t mcmc_steps, double depth);




};


/******************************/
/* IMPLEMENTATION STARTS HERE */
/******************************/

template<class Particle>
Rep<Particle>::Rep(size_t num_particles, size_t mcmc_steps, double depth)
:num_particles(num_particles)
,mcmc_steps(mcmc_steps)
,depth(depth)
{

}








} // namespace InfoNest

#endif

