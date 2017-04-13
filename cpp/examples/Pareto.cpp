#include "Pareto.h"
#include "Utils.h"

#include <algorithm>

namespace InfoNest
{

Pareto::Pareto()
:top_ten(10)
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

    generate_data(rng);
}

void Pareto::generate_data(RNG& rng)
{
    // All scores
    std::vector<unsigned int> scores((size_t)N);

    // Reciprocal of alpha
    double log_x_min = log(x_min);
    double one_over_alpha = 1.0/alpha;

    // Generate scores
    double e;
    for(unsigned int& score: scores)
    {
        // Generate an exponential
        e = -log(1.0 - rng.rand());
        score = (unsigned int)exp(log_x_min + one_over_alpha*e);
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

 
    return logH;
}

void Pareto::print(std::ostream& out)
{
    out<<x_min<<' '<<alpha<<' '<<N<<' ';
    for(unsigned int score: top_ten)
        out<<score<<' ';
}

// Difference between maximum observations
// from first half
double Pareto::parameter_distance(const Pareto& pareto1,
                                  const Pareto& pareto2)
{
    return 0.0;
}


// Difference between maximum observations
// from second half
double Pareto::data_distance(const Pareto& pareto1,
                             const Pareto& pareto2)
{
    return 0.0;
}

double Pareto::joint_distance(const Pareto& pareto1, const Pareto& pareto2)
{
    double d1 = parameter_distance(pareto1, pareto2);
    double d2 = data_distance(pareto1, pareto2);
    return sqrt(d1*d1 + d2*d2);
}

} // namespace InfoNest

