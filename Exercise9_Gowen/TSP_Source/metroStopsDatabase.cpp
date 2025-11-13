#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

#include "metroStopsDatabase.h"

using namespace std;

MetroStopsDatabase::MetroStopsDatabase (const std::string& filename)
{
	loadData(filename);
}

void MetroStopsDatabase::loadData(const std::string& filename)
{
    ifstream file(filename);

    if (!file) {
        cerr << "Error: could not open file\n";
        return;
    }

    string header;
    getline(file, header); // "throwaway" read, to skip the column titles

    Station entry;
    char quote;

	std::string posStr;

	while (
	    file >> quote && quote == '"' &&
	    std::getline(file, entry.stationName, '"') &&

	    file >> quote && quote == '"' &&
	    std::getline(file, entry.stationLabel, '"') &&

	    file >> quote && quote == '"' &&
	    std::getline(file, posStr, '"')
	) {
	    entry.position = std::stod(posStr);
	    stations.push_back(entry);
	}

}

void MetroStopsDatabase::printStations() const
{
    // Table header
    cout << left
         << setw(20) << "Station"
         << setw(25) << "Label"
         << setw(30) << "Position"
         << "\n";

    cout << string(75, '-') << "\n"; // Divider line

    // Table rows
    for (const auto &s : stations) {
        cout << left
             << setw(20) << s.stationName
             << setw(25) << s.stationLabel
             << setw(30) << s.position
             << "\n";
    }
}

const Station& MetroStopsDatabase::getStation(size_t index) const
{
	return stations.at(index);
}

size_t MetroStopsDatabase::size() const
{
	return stations.size();
}







