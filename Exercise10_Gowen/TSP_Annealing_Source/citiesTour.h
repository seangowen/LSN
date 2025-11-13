#ifndef __citiesTour__
#define __citiesTour__

#include <vector>
#include <random>
#include <iostream>

struct City 
{
    int cityLabel;
    double position;
};

class CitiesTour 
{
private:
    std::vector<City> cities;
    std::mt19937 rng;

public:
    CitiesTour(int numCities, unsigned int seed = 12345);

    void printCities() const;
    const std::vector<City>& getCities() const;
};

#endif
