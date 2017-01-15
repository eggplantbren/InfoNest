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
    T = exp(log(1E-3 * t_range) + log(1E3) * rng.rand());
    phi = 2.0 * M_PI * rng.rand();

    for(double& nn : n)
        nn = rng.randn();

    assemble();
}


void Sinusoid::assemble()
{
    double t;
    double omega = 2*M_PI/T; // Angular frequency
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
            T = log(T);
            T += log(1E3) * rng.randh();
            wrap(T, log(1E-3*t_range), log(t_range));
            T = exp(T);
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

} // namespace InfoNest

