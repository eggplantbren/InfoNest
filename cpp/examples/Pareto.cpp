#include "Pareto.h"
#include "Utils.h"

#include <algorithm>
#include <iostream>

namespace InfoNest
{

Pareto::Pareto()
:us(N)
,xs(N)
{

}

void Pareto::generate(RNG& rng)
{
    alpha = exp(rng.randn());

    for(double& u: us)
        u = rng.rand();

    generate_data();
}

void Pareto::generate_data()
{
    double inv_alpha = 1.0/alpha;

    for(size_t i=0; i<N; ++i)
       xs[i] = x_min*pow(1.0 - us[i], -inv_alpha);
}

double Pareto::sum_first_half() const
{
    double tot = 0.0;
    for(size_t i=0; i<N/2; ++i)
        tot += xs[i];
    return tot;
}

double Pareto::sum_second_half() const
{
    double tot = 0.0;
    for(size_t i=N/2; i<N; ++i)
        tot += xs[i];
    return tot;
}


double Pareto::perturb(RNG& rng)
{
    double logH = 0.0;

    // What to change?
    int proposal_type = rng.rand_int(2);

    if(proposal_type == 0)
    {
        // Change alpha (and the data)
        alpha = log(alpha);

        logH -= -0.5*pow(alpha, 2);
        alpha += rng.randh();
        logH += -0.5*pow(alpha, 2);

        alpha = exp(alpha);
    }
    else
    {
        // Change the data
        if(rng.rand() <= 0.5)
        {
            // Perturb one value
            int which = rng.rand_int(N);
            us[which] += rng.randh();
            wrap(us[which], 0.0, 1.0);
        }
        else
        {
            // Regenerate many
            int reps = (int)pow((double)N, rng.rand());
            for(int i=0; i<reps; ++i)
                us[rng.rand_int(N)] = rng.rand();
        }
    }
    generate_data();

    return logH;
}

void Pareto::print(std::ostream& out) const
{
    out << x_min << ' ' << alpha << ' ';
    for(double x: xs)
        out << x << ' ';    
}

double Pareto::parameter_distance(const Pareto& pareto1,
                                  const Pareto& pareto2)
{
    return std::abs(log(pareto2.sum_first_half()/pareto1.sum_first_half()));
}

double Pareto::data_distance(const Pareto& pareto1,
                             const Pareto& pareto2)
{
    return std::abs(log(pareto2.sum_second_half()/pareto1.sum_second_half()));
}

double Pareto::joint_distance(const Pareto& pareto1, const Pareto& pareto2)
{
    double d1 = parameter_distance(pareto1, pareto2);
    double d2 = data_distance(pareto1, pareto2);
    return sqrt(d1*d1 + d2*d2);
}

} // namespace InfoNest

