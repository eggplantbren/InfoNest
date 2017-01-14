#include "Normal.h"

namespace InfoNest
{

Normal::Normal()
{

}


void Normal::generate(RNG& rng)
{
    n1 = rng.randn();
    n2 = rng.randn();
    assemble();
}

double Normal::perturb(RNG& rng)
{
    double logH = 0.0;

    // Choose a coordinate to perturb
    int which = rng.rand_int(2);
    double& n = (which == 0)?(n1):(n2);

    logH -= -0.5*n*n;
    n += rng.randh();
    logH += -0.5*n*n;

    assemble();

    return logH;
}

void Normal::assemble()
{
    x = n1;
    y = x + n2;
}

void Normal::print(std::ostream& out)
{
    out<<x<<' '<<y<<' ';
}

} // namespace InfoNest

