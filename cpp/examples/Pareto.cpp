#include "Pareto.h"
#include "Utils.h"

#include <algorithm>

namespace InfoNest
{

Pareto::Pareto()
:xs(1000)
{

}

void Pareto::generate(RNG& rng)
{
    alpha = 0.01 + 4.99*rng.rand();

    for(double& x: xs)
        x = x_min * pow(rng.rand(), -1.0 / alpha);

    compute_logl();
}

void Pareto::compute_logl()
{
    logl = xs.size()*log(alpha);
    for(double x: xs)
        logl += -(alpha + 1.0)*log(x);
}

double Pareto::perturb_alpha(RNG& rng)
{
    alpha += 4.99*rng.randh();
    wrap(alpha, 0.01, 5.0);

    return 0.0;
}

double Pareto::perturb(RNG& rng)
{
    double logH = 0.0;

    int proposal_type = rng.rand_int(4);

    if(proposal_type == 0)
    {
        double old_alpha = alpha;

        logH += perturb_alpha(rng);

        // Transform data to go with new alpha value
        for(double& x: xs)
        {
            x = pow(x / x_min, -old_alpha);
            x = x_min * pow(x, -1.0 / alpha);
        }

        compute_logl();
    }
    else if(proposal_type == 1)
    {
        logH -= logl;

        logH += perturb_alpha(rng);

        compute_logl();
        logH += logl;
    }
    else if(proposal_type == 2)
    {
        int which = rng.rand_int(xs.size());
        xs[which] = pow(xs[which] / x_min, -alpha);
        xs[which] += rng.randh();
        wrap(xs[which], 0.0, 1.0);
        xs[which] = x_min * pow(xs[which], -1.0 / alpha);

        compute_logl();
    }
    else
    {
        int reps = (int)pow((double)xs.size(), rng.rand());
        int which;
        for(int i=0; i<reps; ++i)
        {
            which = rng.rand_int(xs.size());
            xs[which] = x_min * pow(rng.rand(), -1.0 / alpha);
        }

        compute_logl();
    }

    return logH;
}

void Pareto::print(std::ostream& out)
{
    out<<alpha<<' ';
    for(double x: xs)
        out<<x<<' ';
}

double Pareto::parameter_distance(const Pareto& pareto1,
                                  const Pareto& pareto2)
{
    double tot1 = 0.0;
    for(size_t i=0; i<900; ++i)
        tot1 += pareto1.xs[i];

    double tot2 = 0.0;
    for(size_t i=0; i<900; ++i)
        tot2 += pareto2.xs[i];

    return std::abs(log(tot1/900) - log(tot2/900));
}

double Pareto::data_distance(const Pareto& pareto1,
                             const Pareto& pareto2)
{
    // Average of biggest 10
    std::vector<double> temp = pareto1.xs;
    std::sort(temp.begin(), temp.end());
    double tot1 = 0.0;
    for(size_t i=990; i<temp.size(); ++i)
        tot1 += temp[i];

    temp = pareto2.xs;
    std::sort(temp.begin(), temp.end());
    double tot2 = 0.0;
    for(size_t i=990; i<temp.size(); ++i)
        tot2 += temp[i];

    return std::abs(log(tot1/10) - log(tot2/10));
}

double Pareto::joint_distance(const Pareto& pareto1, const Pareto& pareto2)
{
    double d1 = parameter_distance(pareto1, pareto2);
    double d2 = data_distance(pareto1, pareto2);
    return sqrt(d1*d1 + d2*d2);
}

} // namespace InfoNest

