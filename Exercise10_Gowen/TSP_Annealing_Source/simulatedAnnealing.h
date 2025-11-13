#ifndef __simulatedAnnealing__
#define __simulatedAnnealing__

#include <vector>
#include <random>
#include <string>

class SimulatedAnnealing 
{
    public:
        SimulatedAnnealing(int nCities, unsigned int seed);

        // --- Main SA routine ---
        std::vector<int> run(
            const std::vector<std::vector<double>>& distanceMatrix,
            double T0,
            double alpha,
            int maxIter,
            const std::string& costLogFile,
            const std::string& tourLogFile,
            int costLogInterval = 100,
            int tourLogInterval = 500,
            bool verbose = false,
            std::vector<double>* costEvolution = nullptr // to save each rank's output
        );


        void initializeRandomTour();

        // --- Compute tour cost (2D Euclidean distances) ---
        double computeCost(const std::vector<int>& tour,
                           const std::vector<std::vector<double>>& distanceMatrix) const;
        double computeCost2D(const std::vector<int>& tour, const std::vector<std::vector<double>>& positions) const;

        // --- Distance matrix (2D Euclidean) ---
        std::vector<std::vector<double>> computeDistanceMatrix(const std::vector<double>& positions) const;
        std::vector<std::vector<double>> computeDistanceMatrix(const std::vector<std::vector<double>>& positions);
        std::vector<std::vector<double>> computeDistanceMatrix2D(const std::vector<std::vector<double>>& positions) const;

        std::vector<int> proposeNewTour();

    private:
        int numCities;
        std::vector<int> state;      // current tour (chromosome)
        std::vector<int> bestState;  // best found tour
        double bestCost;

        std::mt19937 rng;
        std::uniform_real_distribution<double> dist;
};



#endif
