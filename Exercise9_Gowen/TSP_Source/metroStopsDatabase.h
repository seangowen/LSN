#ifndef __metroStops__
#define __metroStops__


struct Station
{
	std::string stationName;
	std::string stationLabel;
	double position;
};


class MetroStopsDatabase
{
	private:

	std::vector<Station> stations;

	public:

	// Constructor that takes a file path
	MetroStopsDatabase (const std::string& filename);

	void loadData(const std::string& filename);
	void printStations() const;

	const Station& getStation(size_t index) const;
	size_t size() const;

};



#endif