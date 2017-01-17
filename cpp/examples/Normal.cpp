#include "Normal.h"

namespace InfoNest
{

Normal::Normal()
:xs(100)
{

}


void Normal::generate(RNG& rng)
{
    mu = 10.0 * rng.randn();
    for(double& x: xs)
        x = mu + rng.randn();
}

double Normal::perturb(RNG& rng)
{
    double logH = 0.0;

    int proposal_type = rng.rand_int(4);

    if(proposal_type == 0)
    {

    }
    else if(proposal_type == 1)
    {

    }
    else if(proposal_type == 2)
    {

    }
    else
    {

    }


    return logH;
}

void Normal::print(std::ostream& out)
{
    out<<mu<<' ';
}

double Normal::distance(const Normal& normal1, const Normal& normal2)
{
    return 0.0;
}

} // namespace InfoNest

