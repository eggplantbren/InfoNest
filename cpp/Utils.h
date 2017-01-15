#ifndef InfoNest_Utils
#define InfoNest_Utils

// Useful functions, copied from DNest4

namespace InfoNest
{

// This is non-standard, gcc supports it but mingw64 doesn't,
// so putting it here
#ifndef M_PI
    constexpr double M_PI = 3.141592653589793;
#endif

double mod(double y, double x);
int    mod(int y, int x);
void   wrap(double& x, double min, double max);

} // namespace DNest4

#endif

