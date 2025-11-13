#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numbers>
#include <unordered_set>
#include "geneticAlgorithm.h"
#include "citiesTour.h"

using namespace std;

GeneticAlgorithm::GeneticAlgorithm(unsigned int seed) : rng(seed) {
    //std::cout << "[GA] RNG initialized with seed: " << seed << "\n";
}


// --------------- Population Generation --------------

std::vector<int> GeneticAlgorithm::generateRandomSequence(int numCities) 
{
    std::vector<int> tour;
    tour.push_back(0); // hometown fixed at start

    // Create a list of remaining cities [1, 2, ..., numCities-1]
    std::vector<int> remainingCities(numCities - 1);
    // "iota" fills remainingCities in consecutive increasing values
    std::iota(remainingCities.begin(), remainingCities.end(), 1);

    // Shuffle the remaining cities
    std::shuffle(remainingCities.begin(), remainingCities.end(), rng);

    // Append shuffled cities to the tour
    tour.insert(tour.end(), remainingCities.begin(), remainingCities.end());

    // Close the loop by returning to the hometown
    tour.push_back(0);

    return tour;
}


std::vector<std::vector<int>> GeneticAlgorithm::generatePopulation(int numCities, int populationSize) 
{
    std::vector<std::vector<int>> population;
    population.reserve(populationSize); // Memory pre-allocation

    for (int i = 0; i < populationSize; ++i)
        population.push_back(generateRandomSequence(numCities));

    return population;
}


// --------------- Computing Cost --------------

double GeneticAlgorithm::compute_pairwise_distance(double a, double b) const
{
    const double R = 1.0;
    double diff = std::fabs(a - b); // Using floating point absolute value (fabs)
    double two_pi = 2.0 * std::numbers::pi;
    if (diff > std::numbers::pi) diff = two_pi - diff;  // shortest angular difference
    double arc = R * diff;  // arc length
    return arc;   // length (L)
}


double GeneticAlgorithm::computeCost(const std::vector<double>& tour) const 
{
    double total = 0.0;

    // Note: size_t is a long unsigned int
    for (size_t i = 0; i < tour.size() - 1; ++i) 
        total += compute_pairwise_distance(tour[i], tour[i + 1]);

    // Closing the loop (returning to hometown)
    total += compute_pairwise_distance(tour.back(), tour.front());

    return total;
}

// FUNCTION OVERLOAD
// Compute cost from a sequence of city indices using their reference angular positions
double GeneticAlgorithm::computeCost(const std::vector<int>& tour,
                                     const std::vector<double>& referencePositions) const
{
    // Convert indices into angular positions into a temporary vector
    std::vector<double> angularPostions;
    angularPostions.reserve(tour.size());

    for (int idx : tour) 
    {
        if (idx < 0 || static_cast<size_t>(idx) >= referencePositions.size()) 
        {
            std::cerr << "Warning: invalid city index " << idx << " in tour.\n";
            continue;
        }
        angularPostions.push_back(referencePositions[idx]);
    }

    return computeCost(angularPostions);
}

// FUNCTION OVERLOAD ----- 2D ------
// Compute total tour cost for a 2D tour
double GeneticAlgorithm::computeCost(const std::vector<std::vector<double>>& tour) const
{
    double total = 0.0;
    if (tour.empty()) return total;

    for (size_t i = 0; i < tour.size() - 1; ++i)
    {
        double dx = tour[i][0] - tour[i + 1][0];
        double dy = tour[i][1] - tour[i + 1][1];
        total += std::sqrt(dx*dx + dy*dy);
    }

    // Closing the loop (returning to the starting city)
    double dx = tour.back()[0] - tour.front()[0];
    double dy = tour.back()[1] - tour.front()[1];
    total += std::sqrt(dx*dx + dy*dy);

    return total;
}


// FUNCTION OVERLOAD ----- 2D ------
// Compute cost from a sequence of city indices using their 2D positions
double GeneticAlgorithm::computeCost(const std::vector<int>& tour,
                                    const std::vector<std::vector<double>>& referencePositions) const
{
    if (tour.empty()) return 0.0;
    double totalCost = 0.0;

    for (size_t i = 0; i < tour.size(); ++i) 
    {
        int idx1 = tour[i];
        int idx2 = tour[(i + 1) % tour.size()]; // wrap around to form a loop

        // Safety check
        if (idx1 < 0 || static_cast<size_t>(idx1) >= referencePositions.size() ||
            idx2 < 0 || static_cast<size_t>(idx2) >= referencePositions.size())
        {
            std::cerr << "Warning: invalid city index in tour: "
                      << idx1 << " or " << idx2 << "\n";
            continue;
        }

        const auto& city1 = referencePositions[idx1];
        const auto& city2 = referencePositions[idx2];

        // Euclidean distance
        double dx = city1[0] - city2[0];
        double dy = city1[1] - city2[1];
        totalCost += std::sqrt(dx*dx + dy*dy);
    }

    return totalCost;
}


double GeneticAlgorithm::computeMeanCost(const std::vector<std::pair<int,double>>& costData) const
{
    double meanValue = 0.0;

    for (const auto& entry : costData) 
    {
        meanValue += entry.second;
    }

    if (costData.empty()) return 0.0; // avoid division by zero

    meanValue = meanValue / costData.size();
    return meanValue;
}

// -------------- Fittest Selection --------------
// Roulette wheel (fitness-proportionate) selection
std::vector<std::vector<int>> GeneticAlgorithm::selectFittest(
    const std::vector<std::pair<std::vector<int>, double>>& sortedPopulation,
    int populationSize)
{
    std::vector<std::vector<int>> fittestPopulation;
    fittestPopulation.reserve(populationSize); // full next generation

    // ---- Compute inverse cost as fitness ----
    std::vector<double> fitness(sortedPopulation.size());
    double fitnessSum = 0.0;

    for (size_t i = 0; i < sortedPopulation.size(); ++i) {
        double cost = sortedPopulation[i].second;
        fitness[i] = 1.0 / (cost + 1e-6);  // Avoid division by zero
        fitnessSum += fitness[i];
    }

    // ---- Compute cumulative probabilities ----
    std::vector<double> cumProb(fitness.size());
    double cumsum = 0.0;
    for (size_t i = 0; i < fitness.size(); ++i) {
        cumsum += fitness[i] / fitnessSum;
        cumProb[i] = cumsum;
    }

    // ---- Roulette wheel selection ----
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int n = 0; n < populationSize; ++n) {
        double r = dist(rng); // random number [0, 1]
        auto it = std::lower_bound(cumProb.begin(), cumProb.end(), r);
        size_t idx = std::distance(cumProb.begin(), it);

        // Add the selected tour (vector<int>)
        fittestPopulation.push_back(sortedPopulation[idx].first);
    }

    return fittestPopulation;
}


std::vector<std::vector<int>> GeneticAlgorithm::crossover(const std::vector<std::vector<int>>& fittestPopulation)
{
    std::vector<std::vector<int>> newPopulation;
    int nParents = fittestPopulation.size();

    if (nParents < 2) return newPopulation; // Not enough parents
    
    int numCities = fittestPopulation[0].size();

    // Pair parents sequentially: (0,1), (2,3), ...
    for (int i = 0; i + 1 < nParents; i += 2)
    {
        const auto& parent1 = fittestPopulation[i];
        const auto& parent2 = fittestPopulation[i + 1];

        // --- Child 1: keep parent2 order, insert last-3 cities from parent1 ---
        std::vector<int> child1;
        child1.push_back(parent1.front()); // hometown at start

        // last 3 cities before hometown in parent1
        std::vector<int> segment1(parent1.begin() + (numCities - 3), parent1.begin() + (numCities - 1));

        // copy parent2 cities except those in segment1
        for (int j = 1; j < numCities - 1; j++)  // skip first hometown
        {
            int city = parent2[j];
            if (std::find(segment1.begin(), segment1.end(), city) == segment1.end())
            {
                child1.push_back(city);
            }
        }

        // append segment1 and hometown at end
        child1.insert(child1.end(), segment1.begin(), segment1.end());
        child1.push_back(parent1.back());

        // --- Child 2: anti-symmetric swap ---
        std::vector<int> child2;
        child2.push_back(parent2.front()); // hometown

        std::vector<int> segment2(parent2.begin() + (numCities - 3), parent2.begin() + (numCities - 1));

        // copy parent1 cities except those in segment2
        for (int j = 1; j < numCities - 1; j++)
        {
            int city = parent1[j];
            if (std::find(segment2.begin(), segment2.end(), city) == segment2.end())
            {
                child2.push_back(city);
            }
        }

        child2.insert(child2.end(), segment2.begin(), segment2.end());
        child2.push_back(parent2.back());

        newPopulation.push_back(child1);
        newPopulation.push_back(child2);
    }

    // If odd number of parents, copy the last one unchanged
    if (nParents % 2 == 1)
    {
        newPopulation.push_back(fittestPopulation.back());
    }

    return newPopulation;
}



void GeneticAlgorithm::mutate(std::vector<int>& tour, double mutationRate) 
{
    // Don't mutate the first and last elements (hometown fixed)
    int numCities = tour.size() - 1;
    if (numCities <= 3) return; // too small to mutate meaningfully

    std::uniform_real_distribution<double> prob(0.0, 1.0);
    if (prob(rng) < mutationRate) 
    {
        // Pick two cut points inside the tour (excluding hometowns)
        std::uniform_int_distribution<int> dist(1, numCities - 1);
        int i = dist(rng);
        int j = dist(rng);

        if (i > j) std::swap(i, j); // ensure i < j

        // Reverse the segment between i and j
        std::reverse(tour.begin() + i, tour.begin() + j);
    }
}

void GeneticAlgorithm::mutateAdaptive(std::vector<int>& tour, double baseRate, double bestCost, double meanCost, double tourCost) {
    // Adaptive factor: worse tours get higher mutation
    double factor = (tourCost - bestCost) / (meanCost - bestCost + 1e-6); // avoid division by zero
    factor = std::max(0.0, std::min(factor, 1.0)); // clamp [0,1]

    double mutationRate = baseRate + 0.2 * factor; // can adjust 0.2 to control max increase

    std::uniform_real_distribution<double> prob(0.0, 1.0);
    if (prob(rng) < mutationRate) {
        int numCities = tour.size() - 1; // exclude hometown
        std::uniform_int_distribution<int> dist(1, numCities - 1);
        int i = dist(rng);
        int j = dist(rng);
        if (i != j) std::swap(tour[i], tour[j]);
    }
}

/*
std::vector<int> GeneticAlgorithm::mutateBestTour(const std::vector<int>& bestTour)
{
    std::vector<int> newTour = bestTour;

    int numCities = newTour.size() - 1;
    if (numCities <= 3) return newTour;

    std::uniform_int_distribution<int> dist(1, numCities - 2); // exclude hometowns
    int i = dist(rng);
    int j = dist(rng);

    if (i != j) std::swap(newTour[i], newTour[j]);

    return newTour;
}
*/









