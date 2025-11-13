#ifndef __americanCitiesDatabase__
#define __americanCitiesDatabase__


struct AmericanCity
{
	std::string state;
	std::string capital;
	double longitude;
	double latitude;
	
};


class AmericanCitiesDatabase
{
	private:

	std::vector<AmericanCity> cities;


	public:

	// Constructor that takes a file path
    AmericanCitiesDatabase(const std::string& filename);

	void loadData(const std::string& filename);
	void printCities() const;

	const AmericanCity& getCity(size_t index) const;
	size_t size() const;

};


#endif