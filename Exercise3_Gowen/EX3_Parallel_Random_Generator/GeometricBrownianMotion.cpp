#include <cmath>
#include "GeometricBrownianMotion.h"
#include "random.h"

// Constructor
GeometricBrownianMotion::GeometricBrownianMotion(double S0_, double mu_, double sigma_)
    : S0(S0_), mu(mu_), sigma(sigma_) {}

// Destructor
GeometricBrownianMotion::~GeometricBrownianMotion() {};

// Asset terminal price S(T)
double GeometricBrownianMotion::asset_terminal_price(double T, Random& rnd) const 
{
    return S0 * exp((mu - 0.5 * sigma * sigma) * T + sigma * rnd.Gauss(0, 1) * sqrt(T));
}

// Normal CDF
double GeometricBrownianMotion::N(double x) 
{
    return 0.5 * (1 + erf(x / sqrt(2)));
}

// Black-Scholes call price
double GeometricBrownianMotion::black_scholes_call(double T, double K, double r) const 
{
    double d1 = (log(S0 / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);
    return S0 * N(d1) - K * exp(-r * T) * N(d2);
}

// Black-Scholes put price
double GeometricBrownianMotion::black_scholes_put(double T, double K, double r) const 
{
    double d1 = (log(S0 / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);
    return S0 * (N(d1) - 1) - K * exp(-r * T) * (N(d2) - 1);
}

