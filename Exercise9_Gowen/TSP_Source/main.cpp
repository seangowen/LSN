#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <utility>   // for std::pair
#include "geneticAlgorithm.h"
#include "americanCitiesDatabase.h"
#include "citiesTour.h"
#include "math.h"
#include "random.h"

using namespace std;

// ======== HELPER FUNCTIONS ========

std::ofstream openFileOrExit(const std::string &path) 
{
    std::ofstream file(path);
    if (!file.is_open()) 
    {
        std::cerr << "Error: could not open output file " << path << "\n";
        std::exit(1);
    }
    return file;
}

std::vector<double> loadReferencePositions(const std::string& inputFilename)
{
    std::vector<double> referencePositions;
    std::ifstream file(inputFilename);

    if (!file.is_open()) 
    {
        std::cerr << "Error: could not open input file " << inputFilename << "\n";
        std::exit(1);
    }

    std::string line;
    // Try to skip header if present
    std::getline(file, line);
    if (line.find("City") == std::string::npos) 
    {
        // If no header line, rewind to beginning
        file.clear();
        file.seekg(0);
    }

    while (std::getline(file, line)) 
    {
        std::stringstream ss(line);
        std::string cityIndexStr, angleStr;

        if (std::getline(ss, cityIndexStr, ',') && std::getline(ss, angleStr)) 
        {
            try { referencePositions.push_back(std::stod(angleStr)); } 
            catch (const std::invalid_argument&) 
            {
                std::cerr << "Warning: invalid number in line: " << line << "\n";
            }
        }
    }

    file.close();
    return referencePositions;
}


std::vector<std::vector<double>> loadPlanarReferencePositions(const std::string& inputFilename)
{
    std::vector<std::vector<double>> referencePositions;
    std::ifstream file(inputFilename);

    if (!file.is_open()) 
    {
        std::cerr << "Error: could not open input file " << inputFilename << "\n";
        std::exit(1);
    }

    std::string line;
    // Skip header if present
    std::getline(file, line);
    if (line.find("City") == std::string::npos) 
    {
        // No header, rewind
        file.clear();
        file.seekg(0);
    }

    while (std::getline(file, line)) 
    {
        std::stringstream ss(line);
        std::string cityIndexStr, xStr, yStr;

        if (std::getline(ss, cityIndexStr, ',') &&
            std::getline(ss, xStr, ',') &&
            std::getline(ss, yStr)) 
        {
            try 
            {
                double x = std::stod(xStr);
                double y = std::stod(yStr);
                referencePositions.push_back({x, y});
            } 
            catch (const std::invalid_argument&) 
            {
                std::cerr << "Warning: invalid number in line: " << line << "\n";
            }
        }
        else 
        {
            std::cerr << "Warning: could not parse line: " << line << "\n";
        }
    }

    file.close();
    return referencePositions;
}

// Sort population by ascending cost
std::vector<std::pair<std::vector<int>, double>> computeAndSortPopulationByCost(
    GeneticAlgorithm& ga,
    const std::vector<std::vector<int>>& population,
    const std::vector<double>& referencePositions,
    int topToPrint = 5,
    bool print = false
    )
{
    std::vector<std::pair<std::vector<int>, double>> scoredPopulation;
    scoredPopulation.reserve(population.size());

    // --- Compute cost for each tour ---
    for (const auto& tour : population)
    {
        double cost = ga.computeCost(tour, referencePositions);
        scoredPopulation.push_back({tour, cost});
    }

    // --- Sort by cost ascending ---
    std::sort(scoredPopulation.begin(), scoredPopulation.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    // --- Print top N tours ---
    if (print == true)
    {
        std::cout << "\nTop " << topToPrint << " tours by cost:\n";
        for (int i = 0; i < std::min(topToPrint, static_cast<int>(scoredPopulation.size())); ++i)
        {
            std::cout << "  Rank " << i + 1
                      << " | Cost = " << scoredPopulation[i].second
                      << " | Tour: ";

            for (int city : scoredPopulation[i].first)
                std::cout << city << " ";
            std::cout << "\n";
        }
    }

    return scoredPopulation;
}

// FUNCTION OVERLOAD --- 2D ---
// Sort population by ascending cost (2D positions)
std::vector<std::pair<std::vector<int>, double>> computeAndSortPopulationByCost(
    GeneticAlgorithm& ga,
    const std::vector<std::vector<int>>& population,
    const std::vector<std::vector<double>>& referencePositions, // <-- 2D data
    int topToPrint = 5,
    bool print = false
)
{
    std::vector<std::pair<std::vector<int>, double>> scoredPopulation;
    scoredPopulation.reserve(population.size());

    // --- Compute cost for each tour ---
    for (const auto& tour : population)
    {
        double cost = ga.computeCost(tour, referencePositions); // <-- now uses 2D overload
        scoredPopulation.emplace_back(tour, cost);
    }

    // --- Sort by ascending cost ---
    std::sort(scoredPopulation.begin(), scoredPopulation.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    // --- Optionally print top N tours ---
    if (print)
    {
        std::cout << "\nTop " << topToPrint << " tours by cost:\n";
        for (int i = 0; i < std::min(topToPrint, static_cast<int>(scoredPopulation.size())); ++i)
        {
            std::cout << "  Rank " << i + 1
                      << " | Cost = " << scoredPopulation[i].second
                      << " | Tour: ";

            for (int city : scoredPopulation[i].first)
                std::cout << city << " ";
            std::cout << "\n";
        }
    }

    return scoredPopulation;
}



// ============================= TSP ROUTINES ============================

void oneDim_TSP_Routine()
{
    // ------------  Setting up System and References ---------
    int numCities = 30; // from 0 to 29 (30 cities)
    int populationSize = 20000;
    unsigned int seed = 12345;
    int numGenerations = 1000;   // number of generations to simulate

    cout << "1D-TSP Routine " << endl;
    cout << "Num Cities: " << numCities << endl;
    cout << "Population size: " << populationSize << endl;
    cout << "Number of Generations: " << numGenerations << endl;
    cout << " " << endl;

    // ------------ Loading Reference Tour --------
    vector<double> referenceTour = loadReferencePositions("./Data/linear_cities_database.dat");

    // ------ Printing Reference Tour positions (un-comment for debugging) -----    
    //cout << "Reference tour positions: "<< endl;
    //for (size_t i = 0; i < referenceTour.size(); ++i) { cout << referenceTour[i] << endl; }


    // ------------ Instantiating Genetic Algorithm object ------
    // ------------ and computing reference cost ---------
    GeneticAlgorithm geneticAlgorithm(seed);
    double optimalCost = geneticAlgorithm.computeCost(referenceTour);

    cout << "Cost of reference tour: " << optimalCost << endl;

    // ----- Checking if overloaded function returns the same value as bestCost ---
    // ----- by comparing with a testReferenceTour (0,1,2,..,29) -----
    std::vector<int> testReferenceTour(numCities);
    std::iota(testReferenceTour.begin(), testReferenceTour.end(), 0);
    double testBestCost = geneticAlgorithm.computeCost(testReferenceTour, referenceTour);

    cout << "Cost of TEST reference tour: " << testBestCost << endl;


    // ------------ Initializing and sorting first population by increasing cost -----
    auto population = geneticAlgorithm.generatePopulation(numCities, populationSize);

    auto sortedPopulation = computeAndSortPopulationByCost(geneticAlgorithm, population, referenceTour, 5, true);

    auto bestTour = sortedPopulation.front().first;    // <-- extract vector<int>
    double bestCost = sortedPopulation.front().second; // <-- reuse computed cost (double)


    cout << "Cost of best tour so far tour: " << bestCost << endl;

    // ------------ Open Output File ------------
    auto outFileCost = openFileOrExit("./Data_out/cost_evolution.dat");
    outFileCost << "Optimal cost=," << optimalCost << "\n";
    outFileCost << "Generation,BestCost,MeanCost\n";

    
    for (int gen = 0; gen < numGenerations; gen++)
    {
        // ------ Compute and sort population ------
        auto scoredPopulation = computeAndSortPopulationByCost(
        geneticAlgorithm, population, referenceTour, 5, false);

        // ------ Extract best and mean cost across each generation -----
        double bestCost = scoredPopulation.front().second;

        double meanCost = 0.0;

        for (const auto& p : scoredPopulation) { meanCost += p.second; }
        meanCost /= scoredPopulation.size();


        // ----- Saving to output file ------
        outFileCost << gen << "," << bestCost << "," << meanCost << "\n";

        if (gen % 100 == 0) 
        { 
            cout << "\n=== Generation " << gen << " ===\n";
            cout << "Best cost: " << bestCost 
                  << " | Mean cost: " << meanCost << "\n";
            cout << "Best Tour: ";
            for (int city : scoredPopulation.front().first)
            {
                cout << city << " ";
            }
            cout << "\n";
        }

        // ----- Selecting Fittest population ----
        auto fittestPopulation = geneticAlgorithm.selectFittest(scoredPopulation, populationSize);

        // ----- Crossover ----
        population = geneticAlgorithm.crossover(fittestPopulation);
        
        // ----- Some Chromosomes of the population are mutated ----
        for (auto& p : population) { geneticAlgorithm.mutate(p, 0.05); }

    }

    cout << "Routine completed! " << endl;
    cout << "\nSaved cost evolution to ./Data_out/cost_evolution.dat\n";

}

void twoDim_TSP_Routine()
{
    // ------------  Setting up System and References ---------
    int numCities = 30; // from 0 to 29 (30 cities)
    int populationSize = 20000;
    unsigned int seed = 12345;
    int numGenerations = 1000;   // number of generations to simulate

    cout << "2D-TSP Routine " << endl;
    cout << "Num Cities: " << numCities << endl;
    cout << "Population size: " << populationSize << endl;
    cout << "Number of Generations: " << numGenerations << endl;
    cout << " " << endl;

    // ------------ Loading 2D Reference Tour --------
    auto referenceTour = loadPlanarReferencePositions("./Data/plane_cities_database.dat");

    // Printing referenceTour (uncomment for debugging)
    //for (size_t i = 0; i < referenceTour.size(); ++i) 
    //{
    //    std::cout << "City " << i << ": x=" << referenceTour[i][0]
    //              << ", y=" << referenceTour[i][1] << "\n";
    //}

    // ------------ Instantiating Genetic Algorithm object ------
    // ------------ and computing reference cost ---------
    GeneticAlgorithm geneticAlgorithm(seed);
    double initialCost = geneticAlgorithm.computeCost(referenceTour);

    cout << "Cost of intial tour: " << initialCost << endl;


    // ------------ Initializing and sorting first population by increasing cost -----
    auto population = geneticAlgorithm.generatePopulation(numCities, populationSize);

    auto sortedPopulation = computeAndSortPopulationByCost(geneticAlgorithm, population, referenceTour, 5, true);

    auto bestTour = sortedPopulation.front().first;    // <-- extract vector<int>
    double bestCost = sortedPopulation.front().second; // <-- reuse computed cost (double)


    cout << "Cost of best tour so far tour: " << bestCost << endl;

    
    // ------------ Open Output Files ------------
    auto outFileCost = openFileOrExit("./Data_out/cost_evolution_two_dimensional.dat");
    outFileCost << "Initial cost=," << initialCost << "\n";
    outFileCost << "Generation,BestCost,MeanCost\n";

    auto outFileTours = openFileOrExit("./Data_out/best_tours_two_dimensional.dat");
    outFileTours << "Generation,BestTour" << endl;

    
    for (int gen = 0; gen < numGenerations; gen++)
    {
        // ------ Compute and sort population ------
        auto scoredPopulation = computeAndSortPopulationByCost(
        geneticAlgorithm, population, referenceTour, 5, false);

        // ------ Extract best and mean cost across each generation -----
        double bestCost = scoredPopulation.front().second;

        double meanCost = 0.0;

        for (const auto& p : scoredPopulation) { meanCost += p.second; }
        meanCost /= scoredPopulation.size();


        // ----- Saving to output file ------
        outFileCost << gen << "," << bestCost << "," << meanCost << "\n";

        if (gen % 100 == 0) 
        { 
            cout << "\n=== Generation " << gen << " ===\n";
            cout << "Best cost: " << bestCost 
                  << " | Mean cost: " << meanCost << "\n";
            cout << "Best Tour: ";
            for (int city : scoredPopulation.front().first)
            {
                cout << city << " ";
            }
            cout << "\n";

            // Saving the best tours to the output file
            outFileTours << gen << ",";
            for (int city : scoredPopulation.front().first)
                outFileTours << city << ",";
            outFileTours << "\n";

        }

        // ----- Selecting Fittest population ----
        auto fittestPopulation = geneticAlgorithm.selectFittest(scoredPopulation, populationSize);

        // ----- Crossover ----
        population = geneticAlgorithm.crossover(fittestPopulation);
        
        // ----- Some Chromosomes of the population are mutated ----
        for (auto& p : population) { geneticAlgorithm.mutate(p, 0.05); }

    }

    cout << "Routine completed! " << endl;
    //cout << "\nSaved cost evolution to ./Data_out/cost_evolution.dat\n";

}


// ================================ MAIN =================================

int main() 
{
    int choice;  // variable to store user input

    cout << "--------------- GENETIC ALGORITHM --------------- " << endl;
    cout << "Select which genetic algorithm routine to run: " << endl;
    cout << "1: one-dimensional TSP " << endl;
    cout << "2: two-dimensional TSP " << endl;

    while (true) 
    {

        cout << "Enter your choice: ";

        if (cin >> choice) 
        {
            if (choice == 1) 
            {
                cout << "You selected the one-dimensional TSP.\n";
                cout << " " << endl;
                oneDim_TSP_Routine();
                break;
            } 
            else if (choice == 2) {
                cout << "You selected the two-dimensional TSP.\n";
                cout << " " << endl;
                twoDim_TSP_Routine();
                break;
            } 
            else { cout << "Invalid choice. Please enter 1 or 2.\n"; }
        } 
        else 
        {
            // Handle non-numeric input
            cout << "Invalid input. Please enter a number (1 or 2).\n";
            cin.clear(); // reset error flags
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
        }
    }


    return 0;
}
