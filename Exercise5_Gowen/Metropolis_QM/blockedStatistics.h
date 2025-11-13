#ifndef _blockedStatistics_h_
#define _blockedStatistics_h_

#include "random.h"
#include "measure.h"

class BlockedStatistics: public Random
{
	private:

	Random _rnd;

	public:

	BlockedStatistics();
	~BlockedStatistics();

	void blocking(int M, int N, Measure *measure, std::string filename);
	double uniform_sampling(int min, int max);
	double error(double val, double val2, unsigned int k);
	double mean(std::vector<double> vec);
	double chisquared(std::vector<double> vec, double EV, bool approx);

};


#endif