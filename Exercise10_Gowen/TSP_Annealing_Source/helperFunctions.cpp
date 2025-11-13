#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <utility>   // for std::pair
#include "geneticAlgorithm.h"
#include "helperFunctions.h"

using namespace std;



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
    int topToPrint,
    bool print
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
    int topToPrint,
    bool print
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

std::vector<std::vector<int>> loadReferenceFromFile(
    const std::string& filename,
    std::vector<double>& angles)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file " << filename << "\n";
        std::exit(1);
    }

    std::string line;
    // Try to skip header
    std::getline(file, line);
    if (line.find("City") == std::string::npos)
    {
        // No header — rewind
        file.clear();
        file.seekg(0);
    }

    std::vector<int> cityLabels;
    angles.clear();

    // Read each line
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string cityStr, angleStr;

        if (std::getline(ss, cityStr, ',') && std::getline(ss, angleStr))
        {
            try
            {
                int city = std::stoi(cityStr);
                double angle = std::stod(angleStr);
                cityLabels.push_back(city);
                angles.push_back(angle);
            }
            catch (const std::invalid_argument&)
            {
                std::cerr << "Warning: invalid number in line: " << line << "\n";
            }
        }
    }
    file.close();

    // Create Ising-like matrix (identity)
    size_t numCities = cityLabels.size();
    std::vector<std::vector<int>> isingMatrix(numCities, std::vector<int>(numCities, 0));

    for (size_t i = 0; i < numCities; ++i)
        isingMatrix[i][i] = 1;  // diagonal ones

    return isingMatrix;
}



