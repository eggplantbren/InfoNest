#include "Sinusoid.h"
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



} // namespace InfoNest

