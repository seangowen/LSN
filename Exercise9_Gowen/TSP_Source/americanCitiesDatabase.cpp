#include <iostream>
#include <fstream>
#include <vector>

#include "americanCitiesDatabase.h"


using namespace std;


AmericanCitiesDatabase::AmericanCitiesDatabase(const std::string& filename)
{
	loadData(filename);
}

void AmericanCitiesDatabase::loadData(const std::string& filename)
{
    ifstream file(filename);

    if (!file) {
        cerr << "Error: could not open file\n";
        return;
    }

    string header;
    getline(file, header); // "throwaway" read, to skip the column titles

    AmericanCity entry;
    char quote;

    while 
    (
        file >> quote && quote == '"' &&
        getline(file, entry.state, '"') &&

        file >> quote && quote == '"' &&
        getline(file, entry.capital, '"') &&

        file >> entry.longitude >> entry.latitude

    ) {  cities.push_back(entry);    }
}

void AmericanCitiesDatabase::printCities() const
{
    for (const auto& s : cities) 
    {
        cout << s.state << " - " << s.capital << " (" << s.longitude << ", " << s.latitude << ")\n";
    }
}

const AmericanCity& AmericanCitiesDatabase::getCity(size_t index) const
{
	return cities.at(index);
}

size_t AmericanCitiesDatabase::size() const 
{
    return cities.size();
}




