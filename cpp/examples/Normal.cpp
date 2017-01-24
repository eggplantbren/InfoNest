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

double Normal::perturb_mu(RNG& rng)
{
    double logH = 0.0;

    logH -= -0.5*pow(mu/10.0, 2);
    mu += 10.0 * rng.randh();
    logH += -0.5*pow(mu/10.0, 2);

    return logH;
}

double Normal::perturb(RNG& rng)
{
    double logH = 0.0;

    int proposal_type = rng.rand_int(4);

    if(proposal_type == 0)
    {
        // Perturb parameters, changing data along with it
        double old_mu = mu;

        logH += perturb_mu(rng);

        double delta_mu = mu - old_mu;
        for(double& x: xs)
            x += delta_mu;

        calculate_logl();
    }
    else if(proposal_type == 1)
    {
        // Perturb parameters, keeping data constant
        // (aka Metropolis step of the posterior!)
        logH -= logl;

        logH += perturb_mu(rng);
        
        calculate_logl();
        logH += logl;
    }
    else if(proposal_type == 2)
    {
        // Change one data point
        int which = rng.rand_int(xs.size());

        logH -= -0.5*pow(xs[which] - mu, 2);
        xs[which] += rng.randh();
        logH += -0.5*pow(xs[which] - mu, 2);

        calculate_logl();
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

        calculate_logl();
    }

    return logH;
}

void Normal::print(std::ostream& out)
{
    out<<mu<<' ';
    for(double x: xs)
        out<<x<<' ';
}

double Normal::parameter_distance(const Normal& normal1,
                                  const Normal& normal2)
{
    return std::abs(normal1.mu - normal2.mu);
}

double Normal::data_distance(const Normal& normal1,
                             const Normal& normal2)
{
    double dsq = 0.0;

    for(size_t i=0; i<normal1.xs.size(); ++i)
        dsq += pow(normal1.xs[i] - normal2.xs[i], 2);

    return sqrt(dsq);
}

double Normal::joint_distance(const Normal& normal1, const Normal& normal2)
{
    double d1 = parameter_distance(normal1, normal2);
    double d2 = data_distance(normal1, normal2);
    return (d1 > d2) ? (d1) : (d2);
}

} // namespace InfoNest

