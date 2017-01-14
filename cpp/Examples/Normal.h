#ifndef InfoNest_Normal
#define InfoNest_Normal

#include "../RNG.h"

namespace InfoNest
{

/*
* A 2D gaussian example.
*/
class Normal
{
    private:
        double x;
        double y;

        // Latent coordinates
        double n1, n2;
        void assemble();

    public:
        Normal();

        void generate(RNG& rng);
        double perturb(RNG& rng);

};

} // namespace InfoNest

#endif

