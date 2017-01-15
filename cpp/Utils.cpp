#include "Utils.h"
#include <cassert>
#include <cmath>

namespace InfoNest
{

double mod(double y, double x)
{
	assert(x > 0.0);
	return (y/x - floor(y/x))*x;
}

void wrap(double& x, double min, double max)
{
	x = InfoNest::mod(x - min, max - min) + min;
}

int mod(int y, int x)
{
	assert(x > 0);
	if(y >= 0)
		return y - (y/x)*x;
	else
		return (x-1) - InfoNest::mod(-y-1, x);
}

} // namespace DNest4

