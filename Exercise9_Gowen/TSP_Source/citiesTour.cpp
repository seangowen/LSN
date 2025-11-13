#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <fstream>
#include "citiesTour.h"

using namespace std;

// Note: initializer list
// When initializing CitiesTour we also initialize one of its member variables rng (an object of mt19937)
CitiesTour::CitiesTour(int numCities, unsigned int seed) : rng(seed) 
{
    std::uniform_real_distribution<double> dist(1.0, 10.0);

    std::vector<double> positions(numCities);
    for (int i = 0; i < numCities; ++i)
        positions[i] = dist(rng);

    // Sort positions in ascending order
    std::sort(positions.begin(), positions.end());

    cities.resize(numCities);
    for (int i = 0; i < numCities; ++i) 
    {
        cities[i].cityLabel = i;
        cities[i].position = positions[i];
    }
}

void CitiesTour::printCities() const 
{
    ofstream outFile("./Data_out/cities_tour_positions.dat");

    if (!outFile.is_open()) 
    { 
        cout << "Error, could not open output file.\n";
        return;
    }
    else
    {
        std::cout << "Cities Tour (Reference Positions):\n";
        outFile << "city_index, " << "x_position" << "\n";
        for (const auto& c : cities) 
        {
            std::cout << "City " << c.cityLabel << " at " << c.position << "\n";
            outFile << c.cityLabel << " , " << c.position << "\n";
        }
    }

    outFile.close();
}

const std::vector<City>& CitiesTour::getCities() const 
{
    return cities;
}
