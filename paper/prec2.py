# Testing the analytical formula currently in the appendix
import numpy as np
import matplotlib.pyplot as plt

# Set up grid
x = np.linspace(-10, 10, 50001)
dx = x[1] - x[0]

# Set up a density
f = (x + 5.0)**3
f[np.abs(x) > 5] = 0
f /= np.trapz(f, x=x)   # Normalise

# Width of interval
L = 1.0

# Calculate the two integrands
integrand_left  = np.zeros(len(x))
integrand_right = np.zeros(len(x))
for i in range(0, len(x)):
    left  = (x >= x[i] - L) & (x <= x[i])
    right = (x >= x[i]) & (x <= x[i] + L)

    Pleft  = np.trapz(f[left],  x=x[left])
    Pright = np.trapz(f[right], x=x[right])
    if f[i] > 0.0:
        integrand_left[i]  = f[i]*np.log(Pleft)
        integrand_right[i] = f[i]*np.log(Pright)
    print(i+1, "/", len(x))

# Do the integrals
Hleft  = np.trapz(integrand_left,  x=x)
Hright = np.trapz(integrand_right, x=x)

# Calculate the difference
d = np.abs((Hright - Hleft)/Hright)
print("Relative difference =", d)

# Plot it
plt.plot(x, f, label="$f(x)$")
plt.plot(x, integrand_left, label="Integrand (left)")
plt.plot(x, integrand_right, label="Integrand (right)")
plt.xlabel("$x$")
plt.ylabel("$y$")
plt.legend()
plt.show()

