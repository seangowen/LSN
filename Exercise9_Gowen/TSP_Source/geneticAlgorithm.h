#ifndef __geneticAlgorithm__
#define __geneticAlgorithm__

#include <vector>
#include <random>

struct City; // forward declaration

class GeneticAlgorithm 
{
	private:
    	std::mt19937 rng;

	public:
	    explicit GeneticAlgorithm(unsigned int seed = std::random_device{}());

	    std::vector<int> generateRandomSequence(int numCities);
	    std::vector<std::vector<int>> generatePopulation(int numCities, int populationSize);

	    double compute_pairwise_distance(double a, double b) const;
	    double computeCost(const std::vector<double>& tour) const;
	    double computeCost(const std::vector<std::vector<double>>& tour) const;
	    double computeCost(const std::vector<int>& tour,
                   const std::vector<double>& referencePositions) const;
	    double computeCost(const std::vector<int>& tour,
                                    const std::vector<std::vector<double>>& referencePositions) const;
	    double computeMeanCost(const std::vector<std::pair<int,double>>& costData) const;

		std::vector<std::vector<int>> selectFittest(
			const std::vector<std::pair<std::vector<int>, double>>& sortedPopulation,
    		int populationSize);

	    std::vector<std::vector<int>> crossover(
	    	const std::vector<std::vector<int>>& fittestPopulation);

	    void mutate(std::vector<int>& tour, double mutationRate);
	    void mutateAdaptive(std::vector<int>& tour, double baseRate, double bestCost, double meanCost, double tourCost);


};

#endif


