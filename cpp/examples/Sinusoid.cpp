#include "Sinusoid.h"

#include "../Utils.h"
#include <cmath>

namespace InfoNest
{

Sinusoid::Sinusoid()
:t(N)
,mu(N)
,y(N)
{
    for(size_t i=0; i<N; ++i)
    {
        // For evenly spaced points
        t[i] = t_min + i * dt;

        // For unevenly spaced points
        // t[i] = t_min + pow((i + 0.5)/N, 3)*t_range;
    }
}

void Sinusoid::generate(RNG& rng)
{
    A = exp(0.1*rng.randn());
    log10_period = -rng.rand();
    phi = 2.0 * M_PI * rng.rand();

    calculate_mu();

    for(size_t i=0; i<N; ++i)
        y[i] = mu[i] + sigma*rng.randn();

    calculate_logl();
}

void Sinusoid::calculate_mu()
{
    double omega = 2*M_PI/pow(10.0, log10_period); // Angular frequency
    for(size_t i=0; i<N; ++i)
        mu[i] = A * sin(omega*t[i] + phi);
}

double Sinusoid::perturb_parameters(RNG& rng)
{
    double logH = 0.0;

    // Perturb one of the three parameters
    int which = rng.rand_int(3);
    if(which == 0)
    {
        A = log(A);

        logH -= -0.5*pow(A/0.1, 2);
        A += 0.1*rng.randh();
        logH += -0.5*pow(A/0.1, 2);

        A = exp(A);
    }
    else if(which == 1)
    {
        log10_period += rng.randh();
        wrap(log10_period, -1.0, 0.0);
    }
    else
    {
        phi += 2 * M_PI * rng.randh();
        wrap(phi, 0.0, 2 * M_PI);
    }

    return logH;
}

void Sinusoid::calculate_logl()
{
    logl = 0.0;
    double C = log(1.0 / sqrt(2.0 * M_PI) / sigma);
    double tau = 1.0 / (sigma * sigma);

    for(size_t i=0; i<N; ++i)
        logl += C - 0.5 * pow(y[i] - mu[i], 2) * tau;
}

double Sinusoid::perturb(RNG& rng)
{
    double logH = 0.0;

    int proposal_type = rng.rand_int(4);

    if(proposal_type == 0)
    {
        // Perturb parameters, changing data along with it
        std::vector<double> mu_old = mu;

        logH += perturb_parameters(rng);
        calculate_mu();

        double n;
        for(size_t i=0; i<N; ++i)
        {
            n = (y[i] - mu_old[i]) / sigma;
            y[i] = mu[i] + sigma * n;
        }

        calculate_logl();
    }
    else if(proposal_type == 1)
    {
        // Perturb parameters, keeping data constant
        // (aka Metropolis step of the posterior!)
        logH -= logl;

        logH += perturb_parameters(rng);

        calculate_mu();
        calculate_logl();

        logH += logl;        
    }
    else if(proposal_type == 2)
    {
        // Just change one datum
        int which = rng.rand_int(N);

        logH -= -0.5*pow((y[which] - mu[which])/sigma, 2);
        y[which] += sigma * rng.randh();
        logH += -0.5*pow((y[which] - mu[which])/sigma, 2);

        calculate_logl();
    }
    else
    {
        // Potentially regenerate many of the data points
        int reps = pow(N, rng.rand());
        int which;
        for(int i=0; i<reps; ++i)
        {
            which = rng.rand_int(N);
            y[which] = mu[which] + sigma * rng.randn();
        }

        calculate_logl();
    }

    return logH;
}

void Sinusoid::print(std::ostream& out)
{
    out<<A<<' '<<log10_period<<' '<<phi<<' ';
    for(double yy: y)
        out<<yy<<' ';
}

double Sinusoid::parameter_distance(const Sinusoid& s1, const Sinusoid& s2)
{
    // For H_{x} where x = log10_period.
    return std::abs(s2.log10_period - s1.log10_period);
}

double Sinusoid::data_distance(const Sinusoid& s1, const Sinusoid& s2)
{
    // For H_{data}
    double dsq = 0.0;

    for(size_t i=0; i<N; ++i)
        dsq += pow(s2.y[i] - s1.y[i], 2);

    return sqrt(dsq);
}

double Sinusoid::joint_distance(const Sinusoid& s1, const Sinusoid& s2)
{
    // For H_{x, data} where x = log10_period.
    double d1 = parameter_distance(s1, s2);
    double d2 = data_distance(s1, s2);
    return sqrt(d1*d1 + d2*d2);
}

} // namespace InfoNest

