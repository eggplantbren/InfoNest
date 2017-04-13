#include "Pareto.h"
#include "Utils.h"

#include <algorithm>

namespace InfoNest
{

Pareto::Pareto()
{

}

void Pareto::generate(RNG& rng)
{
    // p(x_min) ~ log-normal(1000, 5)
    x_min = exp(log(1000.0) + 5.0*rng.randn());

    // p(alpha) ~ log-uniform(1, 5)
    alpha = exp(log(5.0)*rng.rand());

    // p(N) ~ pareto(10, 0.5)T(, 1000000)
    double logN;
    do
    {
        logN = log(10.0) - 2.0*log(1.0 - rng.rand());
        N = exp(logN);
    }while(N > 1000000.0);

    generate_data();
}

void Pareto::generate_data()
{

}

double Pareto::perturb(RNG& rng)
{
    double logH = 0.0;

 
    return logH;
}

void Pareto::print(std::ostream& out)
{
    out<<x_min<<' '<<alpha<<' '<<N<<' ';
}

// Difference between maximum observations
// from first half
double Pareto::parameter_distance(const Pareto& pareto1,
                                  const Pareto& pareto2)
{
    double x1_max = -1E300;
    double x2_max = -1E300;

    for(size_t i=0; i<pareto1.xs.size()/2; ++i)
    {
        if(pareto1.xs[i] > x1_max)
            x1_max = pareto1.xs[i];
        if(pareto2.xs[i] > x2_max)
            x2_max = pareto2.xs[i];
    }

    double delta = log(x2_max) - log(x1_max);
    return std::abs(delta);
}


// Difference between maximum observations
// from second half
double Pareto::data_distance(const Pareto& pareto1,
                             const Pareto& pareto2)
{
    double x1_max = -1E300;
    double x2_max = -1E300;

    for(size_t i=pareto1.xs.size()/2; i<pareto1.xs.size(); ++i)
    {
        if(pareto1.xs[i] > x1_max)
            x1_max = pareto1.xs[i];
        if(pareto2.xs[i] > x2_max)
            x2_max = pareto2.xs[i];
    }

    double delta = log(x2_max) - log(x1_max);
    return std::abs(delta);
}

double Pareto::joint_distance(const Pareto& pareto1, const Pareto& pareto2)
{
    double d1 = parameter_distance(pareto1, pareto2);
    double d2 = data_distance(pareto1, pareto2);
    return sqrt(d1*d1 + d2*d2);
}

} // namespace InfoNest

