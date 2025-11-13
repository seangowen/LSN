#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <utility>
#include "helperFunctions.h"
#include "routineFunctions.h"
#include "geneticAlgorithm.h"
#include "simulatedAnnealing.h"
#include <mpi.h> // Message Passing Interface


using namespace std;

// ==================== GENETIC ALGORITHM ====================
// ***********************************************************

void oneDim_TSP_GA_Routine()
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


void twoDim_TSP_GA_Routine()
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


// =================== SIMULATED ANNEALING ===================
// ***********************************************************

void oneDim_TSP_SA_Routine()
{
    int numCities = 30;
    unsigned int seed = 12345;

    std::cout << "1D-TSP Simulated Annealing Routine\n";
    std::cout << "Num Cities: " << numCities << "\n\n";

    std::vector<double> angles;
    auto referenceTour = loadReferenceFromFile("./Data/linear_cities_database.dat", angles);
    std::cout << "Loaded " << referenceTour.size() << " cities.\n\n";

    // Create SA solver
    SimulatedAnnealing sa(numCities, seed);

    // Compute distance matrix
    auto distanceMatrix = sa.computeDistanceMatrix(angles);

    // Run SA optimization
    double T0 = 50.0;       // Initial temperature
    double alpha = 0.9999;  // Cooling rate
    int maxIter = 80000;  // Nb of iterations

    int costLogInterval = 1000; // Records best cost every 1000 iterations.
    int tourLogInterval = 5000; // Records best tour every 5000 iterations.

    auto bestTour = sa.run(
        distanceMatrix,
        T0, alpha, maxIter,
        "./Data_out/sa_cost_evolution.dat",
        "./Data_out/sa_best_tour.dat",
        costLogInterval, tourLogInterval,
        true // output log
    );

    std::cout << "\nBest tour found:\n";
    for (int city : bestTour) std::cout << city << " ";
    std::cout << bestTour[0];
    std::cout << "\n";

    double bestCost = sa.computeCost(bestTour, distanceMatrix);
    std::cout << "Best cost = " << bestCost << "\n";
}


void twoDim_TSP_SA_Routine()
{
    int numCities = 30;
    unsigned int seed = 12345;

    std::cout << "2D-TSP Simulated Annealing Routine\n";
    std::cout << "Num Cities: " << numCities << "\n\n";

    std::string inputFile = "./Data/plane_cities_database.dat";
    std::vector<std::vector<double>> referencePositions = loadPlanarReferencePositions(inputFile);

    if (referencePositions.empty()) {
        std::cerr << "Error: no city data loaded from " << inputFile << "\n";
        return;
    }

    numCities = static_cast<int>(referencePositions.size());
    std::cout << "Loaded " << numCities << " cities from file.\n\n";

    SimulatedAnnealing sa(numCities, seed);

    // Compute distance matrix directly from 2D coordinates
    auto distanceMatrix = sa.computeDistanceMatrix(referencePositions);


    double T0 = 100.0;        // Initial temperature
    double alpha = 0.9995;    // Cooling rate
    int maxIter = 100000;     // Total number of iterations

    int costLogInterval = 1000; // Log every 1000 iterations
    int tourLogInterval = 5000; // Log tour every 5000 iterations

    auto bestTour = sa.run(
        distanceMatrix,
        T0, alpha, maxIter,
        "./Data_out/sa_cost_evolution_2D.dat",
        "./Data_out/sa_best_tour_2D.dat",
        costLogInterval, tourLogInterval,
        true // output log
    );

    std::cout << "\nBest tour found:\n";
    for (int city : bestTour)
        std::cout << city << " ";
    std::cout << bestTour[0] << "\n";

    double bestCost = sa.computeCost(bestTour, distanceMatrix);
    std::cout << "Best cost = " << bestCost << "\n";
}


// =================== SIMULATED ANNEALING (Parallel Computing ) ===================
// *********************************************************************************


void oneDim_TSP_SA_Routine_MPI()
{
    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    // === ENFORCE 4 MPI RANKS ===
    if (nprocs != 4) {
        if (rank == 0)
            std::cerr << "ERROR: This MPI routine requires 4 ranks.\n"
                      << "Usage: mpirun -np 4 ./TSP\n";
        MPI_Finalize();
        return;
    }

    int numCities = 30;
    unsigned int seed = 12345 + rank;

    if (rank == 0)
        std::cout << "1D-TSP Simulated Annealing MPI Routine\n"
                  << "Num Cities: " << numCities
                  << "\nMPI ranks: " << nprocs << "\n\n";

    std::vector<double> angles;
    auto referenceTour = loadReferenceFromFile("./Data/linear_cities_database.dat", angles);

    SimulatedAnnealing sa(numCities, seed);
    auto distanceMatrix = sa.computeDistanceMatrix(angles);

    double T0 = 50.0;
    double alpha = 0.9999;
    int maxIter = 60000;
    int costLogInterval = 1000;
    int tourLogInterval = 5000;

    // Each rank keeps its cost evolution
    std::vector<double> localCostEvolution;

    // Only rank 0 prints verbose output
    bool verbose = (rank == 0);

    // Run SA
    auto localBestTour = sa.run(
        distanceMatrix,
        T0, alpha, maxIter,
        "", "",                // no per-rank files
        costLogInterval,
        tourLogInterval,
        verbose,
        &localCostEvolution     // capture evolution
    );

    double localBestCost = sa.computeCost(localBestTour, distanceMatrix);
    std::cout << "[Rank " << rank << "] Local best cost = " << localBestCost << "\n";

    // ---- Reduce global best ----
    struct { double value; int rank; } in, out;
    in.value = localBestCost; in.rank = rank;
    MPI_Allreduce(&in, &out, 1, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);

    std::vector<int> globalBestTour;
    if (rank == out.rank) globalBestTour = localBestTour;
    globalBestTour.resize(numCities + 1);
    MPI_Bcast(globalBestTour.data(), numCities + 1, MPI_INT, out.rank, MPI_COMM_WORLD);

    // ---- Gather cost evolutions on rank 0 ----
    int localSize = (int)localCostEvolution.size();
    std::vector<int> recvCounts(nprocs), displs(nprocs);
    MPI_Gather(&localSize, 1, MPI_INT, recvCounts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<double> allCosts;
    if (rank == 0) {
        int total = 0;
        for (int i = 0; i < nprocs; ++i) {
            displs[i] = total;
            total += recvCounts[i];
        }
        allCosts.resize(total);
    }

    MPI_Gatherv(localCostEvolution.data(), localSize, MPI_DOUBLE,
                allCosts.data(), recvCounts.data(), displs.data(),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // ---- Rank 0 writes combined file ----
    if (rank == 0) {
        int numIters = localSize; // assumes all ranks same size
        std::ofstream outFile("./Data_out/sa_cost_evolution_1D_MPI.dat");
        outFile << "Iteration";
        for (int r = 0; r < nprocs; ++r)
            outFile << ",BestCostRank" << r;
        outFile << "\n";

        for (int i = 0; i < numIters; ++i) {
            outFile << i * costLogInterval;
            for (int r = 0; r < nprocs; ++r) {
                int idx = displs[r] + i;
                if (idx < (int)allCosts.size())
                    outFile << "," << allCosts[idx];
                else
                    outFile << ",";
            }
            outFile << "\n";
        }
        outFile.close();

        // Write best overall
        std::cout << "\nBest tour found (MPI):\n";
        for (int c : globalBestTour) std::cout << c << " ";
        std::cout << globalBestTour[0] << "\nBest cost = " << out.value << "\n";

        std::ofstream bestCost("./Data_out/sa_best_cost_1D_MPI.dat");
        bestCost << "BestCost\n" << out.value << "\n";
        bestCost.close();

        std::ofstream bestTour("./Data_out/sa_best_tour_1D_MPI.dat");
        for (int c : globalBestTour) bestTour << c << ",";
        bestTour << "0\n";
        bestTour.close();
    }
}

void twoDim_TSP_SA_Routine_MPI()
{
    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    // === ENFORCE 4 MPI RANKS ===
    if (nprocs != 4) {
        if (rank == 0)
            std::cerr << "ERROR: This MPI routine requires 4 ranks.\n"
                      << "Usage: mpirun -np 4 ./TSP\n";
        MPI_Finalize();
        return;
    }

    unsigned int seed = 12345 + rank;
    std::string inputFile = "./Data/plane_cities_database.dat";
    auto referencePositions = loadPlanarReferencePositions(inputFile);
    int numCities = static_cast<int>(referencePositions.size());

    if (rank == 0)
        std::cout << "2D-TSP Simulated Annealing MPI Routine\n"
                  << "Num Cities: " << numCities
                  << "\nMPI ranks: " << nprocs << "\n\n";

    SimulatedAnnealing sa(numCities, seed);
    auto distanceMatrix = sa.computeDistanceMatrix(referencePositions);

    double T0 = 100.0;
    double alpha = 0.9995;
    int maxIter = 100000;
    int costLogInterval = 1000;
    int tourLogInterval = 5000;

    std::vector<double> localCostEvolution;
    bool verbose = (rank == 0);

    // --- Run SA ---
    auto localBestTour = sa.run(
        distanceMatrix,
        T0, alpha, maxIter,
        "", "",                // skip internal file logs
        costLogInterval,
        tourLogInterval,
        verbose,
        &localCostEvolution
    );

    double localBestCost = sa.computeCost(localBestTour, distanceMatrix);
    std::cout << "[Rank " << rank << "] Local best cost = " << localBestCost << "\n";

    // --- Save each rank’s best tour ---
    {
        // Save tour (no rank number prefix)
        std::string rankTourFile = "./Data_out/sa_best_tour_2D_rank" + std::to_string(rank) + ".dat";
        std::ofstream rankTour(rankTourFile);
        for (int c : localBestTour)
            rankTour << c << ",";
        rankTour << "0\n"; // close loop back to start
        rankTour.close();

        // Save cost
        std::string rankCostFile = "./Data_out/sa_best_cost_2D_rank" + std::to_string(rank) + ".dat";
        std::ofstream rankCost(rankCostFile);
        rankCost << "BestCost\n" << localBestCost << "\n";
        rankCost.close();
    }

    // ---- Reduce global best ----
    struct { double value; int rank; } in, out;
    in.value = localBestCost; in.rank = rank;
    MPI_Allreduce(&in, &out, 1, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);

    std::vector<int> globalBestTour;
    if (rank == out.rank) globalBestTour = localBestTour;
    globalBestTour.resize(numCities + 1);
    MPI_Bcast(globalBestTour.data(), numCities + 1, MPI_INT, out.rank, MPI_COMM_WORLD);

    // ---- Gather cost evolutions on rank 0 ----
    int localSize = (int)localCostEvolution.size();
    std::vector<int> recvCounts(nprocs), displs(nprocs);
    MPI_Gather(&localSize, 1, MPI_INT, recvCounts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<double> allCosts;
    if (rank == 0) {
        int total = 0;
        for (int i = 0; i < nprocs; ++i) {
            displs[i] = total;
            total += recvCounts[i];
        }
        allCosts.resize(total);
    }

    MPI_Gatherv(localCostEvolution.data(), localSize, MPI_DOUBLE,
                allCosts.data(), recvCounts.data(), displs.data(),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // ---- Rank 0 writes combined and global best files ----
    if (rank == 0) {
        int numIters = localSize; // assuming same length per rank
        std::ofstream outFile("./Data_out/sa_cost_evolution_2D_MPI.dat");
        outFile << "Iteration";
        for (int r = 0; r < nprocs; ++r)
            outFile << ",BestCostRank" << r;
        outFile << "\n";

        for (int i = 0; i < numIters; ++i) {
            outFile << i * costLogInterval;
            for (int r = 0; r < nprocs; ++r) {
                int idx = displs[r] + i;
                if (idx < (int)allCosts.size())
                    outFile << "," << allCosts[idx];
                else
                    outFile << ",";
            }
            outFile << "\n";
        }
        outFile.close();

        // Write best overall results
        std::cout << "\nBest tour found (MPI):\n";
        for (int c : globalBestTour) std::cout << c << " ";
        std::cout << globalBestTour[0] << "\nBest cost = " << out.value << "\n";

        std::ofstream bestCost("./Data_out/sa_best_cost_2D_MPI.dat");
        bestCost << "BestCost\n" << out.value << "\n";
        bestCost.close();

        std::ofstream bestTour("./Data_out/sa_best_tour_2D_MPI.dat");
        for (int c : globalBestTour) bestTour << c << ",";
        bestTour << "0\n";
        bestTour.close();
    }
}


