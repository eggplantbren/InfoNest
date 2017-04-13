#include "Pareto.h"
#include "Utils.h"

#include <algorithm>

namespace InfoNest
{

Pareto::Pareto()
:us(100000)
,top_ten(10)
{

}

void Pareto::generate(RNG& rng)
{
    // p(x_min) ~ log-normal(1000, 5)
    x_min = exp(log(1000.0) + 5.0*rng.randn());

    // p(alpha) ~ log-uniform(1, 5)
    alpha = exp(log(5.0)*rng.rand());

    // p(N) ~ pareto(10, 0.5)T(, 100000)
    double logN;
    do
    {
        logN = log(10.0) - 2.0*log(1.0 - rng.rand());
        N = exp(logN);
    }while(N > 100000.0);

    for(double& u: us)
        u = rng.rand();

    generate_data();
}

void Pareto::generate_data()
{
    // All scores
    std::vector<unsigned int> scores((size_t)N);

    // Reciprocal of alpha
    double log_x_min = log(x_min);
    double one_over_alpha = 1.0/alpha;

    // Generate scores
    double e;
    for(size_t i=0; i<scores.size(); ++i)
    {
        // Generate an exponential
        e = -log(1.0 - us[i]);
        scores[i] = (unsigned int)exp(log_x_min + one_over_alpha*e);
    }

    // Find top ten scores, put them in top_ten
    std::sort(scores.begin(), scores.end());
    size_t k=0;
    for(size_t i=scores.size()-10; i<scores.size(); ++i)
        top_ten[k++] = scores[i];
}

double Pareto::perturb(RNG& rng)
{
    double logH = 0.0;

    if(rng.rand() <= 0.5)
    {
        int which = rng.rand_int(3);

        if(which == 0)
        {
            // p(x_min) ~ log-normal(1000, 5)
            x_min = log(x_min);
            logH -= -0.5*pow((x_min - log(1000.0))/5.0, 2);
            x_min += 5.0*rng.randh();
            logH += -0.5*pow((x_min - log(1000.0))/5.0, 2);
            x_min = exp(x_min);
        }
        else if(which == 1)
        {
            alpha = log(alpha);
            alpha += log(5.0)*rng.randh();
            wrap(alpha, 0.0, log(5.0));
            alpha = exp(alpha);
        }
        else
        {
            // p(N) ~ pareto(10, 0.5)T(, 100000)

            // Transform to exponential then uniform
            double e = (log(N) - log(10.0))/2.0;
            double u = 1.0 - exp(-e);

            // Do the move
            u += rng.randh();
            wrap(u, 0.0, 1.0);

            // Transform back to pareto
            e = -log(1.0 - u);
            N = exp(log(10.0) + 2.0*e);

            // Enforce upper limit
            if(N > 100000.0)
                return -1E300;
        }
    }
    else
    {
        if(rng.rand() <= 0.5)
        {
            int which = rng.rand_int(us.size());
            us[which] += rng.randh();
            wrap(us[which], 0.0, 1.0);
        }
        else
        {
            int reps = pow(us.size(), rng.rand());
            int which;
            for(int i=0; i<reps; ++i)
            {
                which = rng.rand_int(us.size());
                us[which] = rng.rand();
            }
        }

    }
    generate_data();

    return logH;
}

void Pareto::print(std::ostream& out)
{
    out<<x_min<<' '<<alpha<<' '<<N<<' ';
    for(unsigned int score: top_ten)
        out<<score<<' ';
}

std::tuple<double, double, double> Pareto::summaries() const
{
    // Summary stats: min, max, mean
    double min = *min_element(top_ten.begin(), top_ten.end());
    double max = *max_element(top_ten.begin(), top_ten.end());

    double tot = 0.0;
    for(double value: top_ten)
        tot += value;
    double mean = tot/top_ten.size();

    return {min, max, mean};
}


double Pareto::parameter_distance(const Pareto& pareto1,
                                  const Pareto& pareto2)
{
    // Difference between N values (rounded down to integers)
    return std::abs((int)pareto2.N - (int)pareto1.N);
}

double Pareto::data_distance(const Pareto& pareto1,
                             const Pareto& pareto2)
{
    double min1, max1, mean1;
    double min2, max2, mean2;
    std::tie(min1, max1, mean1) = pareto1.summaries();
    std::tie(min2, max2, mean2) = pareto2.summaries();

    // Compare on min/max and mean/max
    double dsq = pow(min1/max1 - min2/max2, 2)
                  + pow(mean1/max1 - mean2/max2, 2);
    return sqrt(dsq);
}

double Pareto::joint_distance(const Pareto& pareto1, const Pareto& pareto2)
{
    double d1 = parameter_distance(pareto1, pareto2);
    double d2 = data_distance(pareto1, pareto2);
    return sqrt(d1*d1 + d2*d2);
}

} // namespace InfoNest

