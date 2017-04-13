#include "GroupDiff.h"
#include "../Utils.h"

namespace InfoNest
{

GroupDiff::GroupDiff()
:xs1(100)
,xs2(100)
{

}

void GroupDiff::generate(RNG& rng)
{
    // Generate from prior
    mu1 = rng.randn();
    sigma1 = exp(rng.randn());
    diff_mu = rng.randn();
    diff_log_sigma = 0.1*rng.randn();
    compute_derived_quantities();

    // Build the data
    for(size_t i=0; i<xs1.size(); ++i)
        xs1[i] = mu1 + sigma1*rng.randn();
    for(size_t i=0; i<xs2.size(); ++i)
        xs2[i] = mu2 + sigma2*rng.randn();
    calculate_logl();
}

void GroupDiff::compute_derived_quantities()
{
    mu2 = mu1 + diff_mu * sigma1;
    sigma2 = sigma1 * exp(diff_log_sigma);
}

void GroupDiff::calculate_logl()
{
    logl = 0.0;

    double C   = -0.5*log(2*M_PI) - log(sigma1);
    double tau = 1.0 / (sigma1*sigma1);

    for(double x: xs1)
        logl += C - 0.5*pow(x - mu1, 2)*tau;

    C   = -0.5*log(2*M_PI) - log(sigma2);
    tau = 1.0 / (sigma2*sigma2);

    for(double x: xs2)
        logl += C - 0.5*pow(x - mu2, 2)*tau;
}

double GroupDiff::perturb_parameter(RNG& rng)
{
    double logH = 0.0;

    int which = rng.rand_int(4);
    if(which == 0)
    {
        logH -= -0.5*pow(mu1, 2);
        mu1 += rng.randh();
        logH += -0.5*pow(mu1, 2);
    }
    else if(which == 1)
    {
        sigma1 = log(sigma1);
        logH -= -0.5*pow(sigma1, 2);
        sigma1 += rng.randh();
        logH += -0.5*pow(sigma1, 2);
        sigma1 = exp(sigma1);
    }
    else if(which == 2)
    {
        logH -= -0.5*pow(diff_mu, 2);
        diff_mu += rng.randh();
        logH += -0.5*pow(diff_mu, 2);
    }
    else if(which == 3)
    {
        logH -= -0.5*pow(diff_log_sigma/0.1, 2);
        diff_log_sigma += 0.1*rng.randh();
        logH += -0.5*pow(diff_log_sigma/0.1, 2);
    }

    compute_derived_quantities();
    return logH;
}

double GroupDiff::perturb(RNG& rng)
{
    double logH = 0.0;

    int proposal_type = rng.rand_int(4);

    if(proposal_type == 0)
    {
        // Perturb parameters, changing data along with it

        // First, standardise all the data
        for(size_t i=0; i<xs1.size(); ++i)
            xs1[i] = (xs1[i] - mu1) / sigma1;
        for(size_t i=0; i<xs2.size(); ++i)
            xs2[i] = (xs2[i] - mu2) / sigma2;

        // Change the parameters
        logH += perturb_parameter(rng);

        // Rebuild the data
        for(size_t i=0; i<xs1.size(); ++i)
            xs1[i] = mu1 + sigma1*xs1[i];
        for(size_t i=0; i<xs2.size(); ++i)
            xs2[i] = mu2 + sigma2*xs2[i];

        calculate_logl();
    }
    else if(proposal_type == 1)
    {
        // Perturb parameters, keeping data constant
        // (aka Metropolis step of the posterior!)
        logH -= logl;
        logH += perturb_parameter(rng);
        calculate_logl();
        logH += logl;
    }
    else if(proposal_type == 2)
    {
        // Change one data point
        int choice = rng.rand_int(2);
        double mu, sigma;
        std::vector<double>* xs;

        if(choice == 0)
        {
            mu = mu1;
            sigma = sigma1;
            xs = &xs1;
        }
        else
        {
            mu = mu2;
            sigma = sigma2;
            xs = &xs2;
        }

        int which = rng.rand_int(xs->size());
        logH -= -0.5*pow(((*xs)[which] - mu)/sigma, 2);
        (*xs)[which] += sigma*rng.randh();
        logH += -0.5*pow(((*xs)[which] - mu)/sigma, 2);
        calculate_logl();
    }
    else
    {
        // Regenerate one or many data points
        int choice = rng.rand_int(2);
        double mu, sigma;
        std::vector<double>* xs;

        if(choice == 0)
        {
            mu = mu1;
            sigma = sigma1;
            xs = &xs1;
        }
        else
        {
            mu = mu2;
            sigma = sigma2;
            xs = &xs2;
        }

        int reps = (int)pow((double)xs->size(), rng.rand());
        int which;
        for(int i=0; i<reps; ++i)
        {
            which = rng.rand_int(xs->size());
            (*xs)[which] = mu + sigma * rng.randn();
        }

        calculate_logl();
    }

    return logH;
}

void GroupDiff::print(std::ostream& out) const
{
    out<<mu1<<' '<<sigma1<<' '<<diff_mu<<' '<<diff_log_sigma<<' ';
    for(double x: xs1)
        out<<x<<' ';
    for(double x: xs2)
        out<<x<<' ';
}

double GroupDiff::parameter_distance(const GroupDiff& g1,
                                     const GroupDiff& g2)
{
    return std::abs(g1.diff_mu - g2.diff_mu);
}

double GroupDiff::data_distance(const GroupDiff& g1,
                                const GroupDiff& g2)
{
    double dsq = 0.0;

    for(size_t i=0; i<g1.xs1.size(); ++i)
        dsq += pow(g1.xs1[i] - g2.xs1[i], 2);

    for(size_t i=0; i<g2.xs2.size(); ++i)
        dsq += pow(g1.xs2[i] - g2.xs2[i], 2);

    return sqrt(dsq);
}

double GroupDiff::joint_distance(const GroupDiff& g1,
                                 const GroupDiff& g2)
{
    double d1 = parameter_distance(g1, g2);
    double d2 = data_distance(g1, g2);
    return sqrt(d1*d1 + d2*d2);
}

} // namespace InfoNest

