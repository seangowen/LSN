#ifndef GEOMETRIC_BROWNIAN_MOTION_H
#define GEOMETRIC_BROWNIAN_MOTION_H

#include "random.h"

class GeometricBrownianMotion 
{

private:
    double S0;     // Initial price
    double mu;     // Drift (risk-free rate for risk-neutral measure)
    double sigma;  // Volatility

public:
    GeometricBrownianMotion(double S0, double mu, double sigma);
    ~GeometricBrownianMotion();

    double asset_terminal_price(double T, Random& rnd) const;
    double black_scholes_call(double T, double K, double r) const;
    double black_scholes_put(double T, double K, double r) const;

    // Helper for normal CDF
    static double N(double x);

};

#endif
