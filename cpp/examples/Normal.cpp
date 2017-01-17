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

    calculate_logl();
}

void Normal::calculate_logl()
{
    logl = 0.0;

    double C = -0.5*log(2*M_PI);
    for(double x: xs)
        logl += C - 0.5*pow(x - mu, 2);
}

double Normal::perturb(RNG& rng)
{
    double logH = 0.0;

    int proposal_type = rng.rand_int(4);

    if(proposal_type == 0)
    {
        // Perturb parameters, changing data along with it
        double old_mu = mu;

        logH -= -0.5*pow(mu/10.0, 2);
        mu += 10.0 * rng.randh();
        logH += -0.5*pow(mu/10.0, 2);

        double delta_mu = mu - old_mu;
        for(double& x: xs)
            x += delta_mu;

    }
    else if(proposal_type == 1)
    {
        // Perturb parameters, keeping data constant
        // (aka Metropolis step of the posterior!)
    }
    else if(proposal_type == 2)
    {
        // Change one data point
        int which = rng.rand_int(xs.size());
//        logH -= -0.5*pow(
    }
    else
    {
        // Regenerate one or many data points
        int reps = (int)pow((double)xs.size(), rng.rand());
        int which;
        for(int i=0; i<reps; ++i)
        {
            which = rng.rand_int(xs.size());
            xs[which] = mu + rng.randn();
        }
    }

    calculate_logl();

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

