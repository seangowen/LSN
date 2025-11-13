#include "simulatedAnnealing.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <cmath>

// --- Constructor ---
SimulatedAnnealing::SimulatedAnnealing(int nCities, unsigned int seed)
    : numCities(nCities), rng(seed), dist(0.0, 1.0)
{
    initializeRandomTour();
}

// --- Initialize random permutation, keep 0 at start ---
void SimulatedAnnealing::initializeRandomTour()
{
    // First and last elements are 0 (Hometown)
    state.resize(numCities);
    std::iota(state.begin(), state.end(), 0);

    // Shuffle positions 1..N-1 only
    std::shuffle(state.begin() + 1, state.end(), rng);
}

// --- Distance matrix (1D circle) ---
std::vector<std::vector<double>> SimulatedAnnealing::computeDistanceMatrix(const std::vector<double>& positions) const
{
    std::vector<std::vector<double>> distanceMatrix(numCities, std::vector<double>(numCities, 0.0));
    for (int i = 0; i < numCities; ++i)
        for (int j = 0; j < numCities; ++j) {
            double diff = std::fabs(positions[i] - positions[j]);
            distanceMatrix[i][j] = std::min(diff, 2 * M_PI - diff);
        }
    return distanceMatrix;
}

// --- OVERLOAD: Distance matrix (2D) ---
std::vector<std::vector<double>> SimulatedAnnealing::computeDistanceMatrix(
    const std::vector<std::vector<double>>& positions)
{
    int n = positions.size();
    std::vector<std::vector<double>> dist(n, std::vector<double>(n, 0.0));

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dx = positions[i][0] - positions[j][0];
            double dy = positions[i][1] - positions[j][1];
            double d = std::sqrt(dx * dx + dy * dy);
            dist[i][j] = dist[j][i] = d;
        }
    }

    return dist;
}

// --- Compute tour cost ---
double SimulatedAnnealing::computeCost(const std::vector<int>& tour,
                                       const std::vector<std::vector<double>>& distanceMatrix) const
{
    double total = 0.0;
    for (int i = 0; i < numCities - 1; ++i)
        total += distanceMatrix[tour[i]][tour[i + 1]];
    total += distanceMatrix[tour.back()][tour.front()]; // return to start
    return total;
}

// =========================== 2D Case ===========================

// --- Distance matrix (2D Euclidean) ---
std::vector<std::vector<double>> SimulatedAnnealing::computeDistanceMatrix2D(
    const std::vector<std::vector<double>>& positions) const
{
    std::vector<std::vector<double>> distanceMatrix(numCities, std::vector<double>(numCities, 0.0));

    for (int i = 0; i < numCities; ++i)
        for (int j = 0; j < numCities; ++j) {
            double dx = positions[i][0] - positions[j][0];
            double dy = positions[i][1] - positions[j][1];
            distanceMatrix[i][j] = std::sqrt(dx * dx + dy * dy);
        }
    return distanceMatrix;
}

// --- Compute tour cost (2D Euclidean distances) ---
double SimulatedAnnealing::computeCost2D(const std::vector<int>& tour,
                                         const std::vector<std::vector<double>>& positions) const
{
    double total = 0.0;
    for (int i = 0; i < numCities - 1; ++i) {
        int a = tour[i];
        int b = tour[i + 1];
        double dx = positions[a][0] - positions[b][0];
        double dy = positions[a][1] - positions[b][1];
        total += std::sqrt(dx * dx + dy * dy);
    }
    // close the loop
    int last = tour.back(), first = tour.front();
    double dx = positions[last][0] - positions[first][0];
    double dy = positions[last][1] - positions[first][1];
    total += std::sqrt(dx * dx + dy * dy);

    return total;
}

// ====================== CORE SA ==========================

// --- Propose new tour using 2-opt ---
std::vector<int> SimulatedAnnealing::proposeNewTour()
{
    std::vector<int> newTour = state;
    if (numCities <= 2) return newTour;

    // Pick two indices i < j in 1..N-1 (excluding city 0)
    int i = 1 + rng() % (numCities - 2);
    int j = i + 1 + rng() % (numCities - i - 1);

    std::reverse(newTour.begin() + i, newTour.begin() + j + 1);
    return newTour;
}

// --- Run simulated annealing ---
std::vector<int> SimulatedAnnealing::run(
    const std::vector<std::vector<double>>& distanceMatrix,
    double T0,
    double alpha,
    int maxIter,
    const std::string& costLogFile,
    const std::string& tourLogFile,
    int costLogInterval,
    int tourLogInterval,
    bool verbose,
    std::vector<double>* costEvolution // to save each rank's output
)
{
    double T = T0;
    double currentCost = computeCost(state, distanceMatrix);

    bestState = state;
    bestCost = currentCost;

    if (costEvolution) costEvolution->clear();

    std::ofstream costLog;
    std::ofstream tourLog;

    if (!costLogFile.empty()) {
        costLog.open(costLogFile);
        costLog << "Iteration,BestCost\n";
    }
    if (!tourLogFile.empty()) {
        tourLog.open(tourLogFile);
        tourLog << "Iteration,BestCost,Tour\n";
    }

    for (int iter = 0; iter < maxIter; ++iter) {
        auto newTour = proposeNewTour();
        double newCost = computeCost(newTour, distanceMatrix);
        double dE = newCost - currentCost;

        // --- Metropolis acceptance rule ---
        if (dE < 0 || dist(rng) < std::exp(-dE / T)) {
            state = newTour;
            currentCost = newCost;

            if (newCost < bestCost) {
                bestCost = newCost;
                bestState = newTour;
            }
        }

        // --- Periodic logging ---
        if (iter % costLogInterval == 0 || iter == maxIter - 1) {
            if (costLog.is_open())
                costLog << iter << "," << bestCost << "\n";

            // Always store evolution (for later MPI gathering)
            if (costEvolution)
                costEvolution->push_back(bestCost);
        }

        if (verbose && (iter % tourLogInterval == 0 || iter == maxIter - 1)) {
            if (tourLog.is_open()) {
                tourLog << iter << "," << bestCost << ",";
                for (int c : bestState) tourLog << c << ",";
                tourLog << "0\n"; // return to start
            }

            std::cout << "Iter " << iter
                      << " | Best cost = " << bestCost
                      << " | Tour: ";
            for (int c : bestState) std::cout << c << " ";
            std::cout << "0\n";
        }

        // --- Cooling schedule ---
        T *= alpha;
        T = std::max(T, 1e-6);
    }

    if (costLog.is_open()) costLog.close();
    if (tourLog.is_open()) tourLog.close();

    return bestState;
}

