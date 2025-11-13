#ifndef __helperFunctions__
#define __helperFunctions__

class GeneticAlgorithm; // Forward declaration of a class

std::ofstream openFileOrExit(const std::string &path);
std::vector<double> loadReferencePositions(const std::string& inputFilename);
std::vector<std::vector<double>> loadPlanarReferencePositions(const std::string& inputFilename);

// Sort population by ascending cost
std::vector<std::pair<std::vector<int>, double>> computeAndSortPopulationByCost(
    GeneticAlgorithm& ga,
    const std::vector<std::vector<int>>& population,
    const std::vector<double>& referencePositions,
    int topToPrint = 5,
    bool print = false
    );

// FUNCTION OVERLOAD --- 2D ---
// Sort population by ascending cost (2D positions)
std::vector<std::pair<std::vector<int>, double>> computeAndSortPopulationByCost(
    GeneticAlgorithm& ga,
    const std::vector<std::vector<int>>& population,
    const std::vector<std::vector<double>>& referencePositions,
    int topToPrint = 5,
    bool print = false
);


// Reads "City_label,Angle_radians"
std::vector<std::vector<int>> loadReferenceFromFile(
    const std::string& filename,
    std::vector<double>& angles
);

#endif