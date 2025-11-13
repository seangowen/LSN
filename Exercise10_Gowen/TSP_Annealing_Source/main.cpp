#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <utility>   // for std::pair
#include <mpi.h>
#include <limits>
#include "geneticAlgorithm.h"
#include "citiesTour.h"
#include "math.h"
#include "simulatedAnnealing.h"
#include "routineFunctions.h"
#include "helperFunctions.h"


using namespace std;


int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    // ---- MPI EXECUTION ----
    if (nprocs > 1)
    {
        // Handle mode argument: default to 1D
        string mode = "1D";
        if (argc > 1)
            mode = argv[1];

        if (rank == 0)
        {
            cout << "====================================\n";
            cout << " MPI Execution Detected (" << nprocs << " ranks)\n";
            cout << " Mode: " << mode << "\n";
            cout << "====================================\n\n";
        }

        if (mode == "1D")
        {
            oneDim_TSP_SA_Routine_MPI();
        }
        else if (mode == "2D")
        {
            twoDim_TSP_SA_Routine_MPI();
        }
        else
        {
            if (rank == 0)
                cerr << "ERROR: Unknown mode \"" << mode
                     << "\".\nUsage: mpirun -np 4 ./TSP [1D|2D]\n";
        }
    }
    else
    {
        // ---- SINGLE-PROCESS EXECUTION ----
        if (rank == 0)
        {
            int choice;
            cout << "--------------- SOLVING THE TSP ------------------ \n";
            cout << "Select which routine to run: \n";
            cout << "1: one-dimensional TSP Genetic Algorithm Routine\n";
            cout << "2: two-dimensional TSP Genetic Algorithm Routine\n";
            cout << "3: one-dimensional TSP Simulated Annealing Routine\n";
            cout << "4: two-dimensional TSP Simulated Annealing Routine\n";

            while (true)
            {
                cout << "Enter your choice: ";
                if (cin >> choice && choice >= 1 && choice <= 4) break;
                cout << "Invalid input. Please enter a number [1 to 4].\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            switch (choice)
            {
                case 1: oneDim_TSP_GA_Routine(); break;
                case 2: twoDim_TSP_GA_Routine(); break;
                case 3: oneDim_TSP_SA_Routine(); break;
                case 4: twoDim_TSP_SA_Routine(); break;
            }
        }
    }

    MPI_Finalize();
    return 0;
}

