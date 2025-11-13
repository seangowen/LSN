#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <numbers>
#include "random.h"
#include "ProbabilityDistribution.h"


using namespace std;


ProbabilityDistribution :: ProbabilityDistribution(){}
ProbabilityDistribution :: ~ProbabilityDistribution(){}

double ProbabilityDistribution :: exponential(double random_uniform, double lambda)
{
	return - log(1.0 - random_uniform) / lambda;
}

double ProbabilityDistribution :: cauchylorentz(double random_uniform, double gamma, double mu)
{
	double pi = std::numbers::pi;
	return gamma * std::tan((random_uniform-0.5)*pi) + mu;
}