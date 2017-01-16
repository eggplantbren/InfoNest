#include "Sinusoid.h"

#include "../Utils.h"
#include <cmath>

namespace InfoNest
{

Sinusoid::Sinusoid()
:n(N)
,y(N)
{

}

void Sinusoid::generate(RNG& rng)
{
    A = -log(1.0 - rng.rand());
    log10_period = log10(1E-3 * t_range) + 3.0 * rng.rand();
    phi = 2.0 * M_PI * rng.rand();

    for(double& nn : n)
        nn = rng.randn();

    assemble();
}


void Sinusoid::assemble()
{
    double t;
    double omega = 2*M_PI/pow(10.0, log10_period); // Angular frequency
    for(size_t i=0; i<N; ++i)
    {
        t = t_min + i * dt;
        y[i] = A * sin(omega*t + phi) + sigma * n[i];
    }
}

double Sinusoid::perturb(RNG& rng)
{
    double logH = 0.0;

    if(rng.rand() < 0.5)
    {
        // Perturb one of the three parameters
        int which = rng.rand_int(3);
        if(which == 0)
        {
            A = 1.0 - exp(-A);
            A += rng.randh();
            wrap(A, 0.0, 1.0);
            A = -log(1.0 - A);
        }
        else if(which == 1)
        {
            log10_period += 3.0 * rng.randh();
            wrap(log10_period, log10(1E-3 * t_range), log10(t_range));
        }
        else
        {
            phi += 2 * M_PI * rng.randh();
            wrap(phi, 0.0, 2 * M_PI);
        }
    }
    else
    {
        // Perturb some of the ns.
        if(rng.rand() <= 0.5)
        {
            // Just change one
            int which = rng.rand_int(N);
            logH -= -0.5*pow(n[which], 2);
            n[which] += rng.randh();
            logH += -0.5*pow(n[which], 2);
        }
        else
        {
            // Potentially regenerate many
            int reps = pow(N, rng.rand());
            for(int i=0; i<reps; ++i)
                n[rng.rand_int(N)] = rng.randn();
        }

    }

    assemble();

    return logH;
}

void Sinusoid::print(std::ostream& out)
{
    out<<A<<' '<<log10_period<<' '<<phi<<' ';
    for(double yy: y)
        out<<yy<<' ';
}

double Sinusoid::distance1(const Sinusoid& s1, const Sinusoid& s2)
{
    // For H_{x} where x = log10_period.
    return std::abs(s2.log10_period - s1.log10_period);
}

double Sinusoid::distance2(const Sinusoid& s1, const Sinusoid& s2)
{
    // For H_{data}
    double rms = 0.0;

    for(size_t i=0; i<N; ++i)
        rms += pow(s2.y[i] - s1.y[i], 2);
    rms = sqrt(rms / N);

    return rms;
}

double Sinusoid::distance3(const Sinusoid& s1, const Sinusoid& s2)
{
    // For H_{x, data} where x = log10_period.
    double d1 = distance1(s1, s2);
    double d2 = distance2(s1, s2);
    return (d2 > d1) ? (d2) : (d1);
}


double Sinusoid::distance(const Sinusoid& s1, const Sinusoid& s2)
{
    return distance1(s1, s2);
}

} // namespace InfoNest

