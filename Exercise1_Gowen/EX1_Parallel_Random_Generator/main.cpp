/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "math.h"
#include "random.h"
#include "MonteCarloIntegration.h"
#include "ProbabilityDistribution.h"
#include "Buffon.h"

using namespace std;


int main (int argc, char *argv[]){

    Random rnd;
    int seed[4];
    int p1, p2;
    ifstream Primes("Primes");
    if (Primes.is_open()){
      Primes >> p1 >> p2 ;
    } else cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();

    ifstream input("seed.in");
    string property;
    if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
    } else cerr << "PROBLEM: Unable to open seed.in" << endl;

    // output for debugging
    // for(int i=0; i<20; i++) {   cout << rnd.Rannyu() << endl;   }

    // ---------- EX.1.1.1 Monte Carlo Integration --------
   

    int M = 100000;      // Total number of throws
    int N = 100;         // Number of blocks
    int L = M/N;         // Throws per block (M as a multiple of N)

    cout << "------- MONTE CARLO INTEGRATION ------" << endl;
    cout << "Total number of throws: " << M << endl;
    cout << "Number of blocks: " << N << endl;
    cout << "Throws per block: " << L << endl;

    vector<double> ave(N,0);  // Average value of the random variable on each block
    vector<double> ave2(N,0); // Average value of the squares of the random variables on each block

    vector<double> sumProg(N,0);  // Progressive sum of the average values
    vector<double> sumProg2(N,0); // Progressive sum of the squares of the average values


    MonteCarloIntegration Integral;

    cout << "Computing Integral 1 .." << endl;
    Integral.firstIntegral(M, N, L, rnd, ave, ave2, sumProg, sumProg2);

    // ---------- EX.1.1.2 Monte Carlo Integration --------

    cout << "Computing Integral 2 .." << endl;
    Integral.secondIntegral(M, N, L, rnd, ave, ave2, sumProg, sumProg2);

    cout << "...done!" << endl;
    cout << " " << endl;


    // ---------- EX.1.1.3 Chi-Squared Test ---------------

    M = 100;       // number of sub-intervals
    N = 10000;     // numbers per test
    int nb_chi2_test = 100;

    double E = double(N) / M;   // expected count per interval
    double chi_squared;         

    cout << "------- CHI-SQUARED TEST ------" << endl;
    cout << "Number of sub-intervals: " << M << endl;
    cout << "Numbers per test: " << N << endl;
    cout << "Number of tests: " << nb_chi2_test << endl;

    ofstream chi_squared_outfile("./Chi_Squared_output_files/chi_squared.dat");

    for (int t = 0; t < nb_chi2_test; t++) 
    {
        // reset counts
        vector<int> counts(M, 0);

        // generate N random numbers and tally counts
        for (int j = 0; j < N; j++) 
        {
            double r = rnd.Rannyu();
            int index = int(r * M); // interval (0 to M-1)
            counts[index]++;
        }

        // compute chi-squared
        chi_squared = 0.0;
        for (int i = 0; i < M; i++) { chi_squared += pow(counts[i] - E, 2) / E; }

        chi_squared_outfile << chi_squared << endl;
    }

    cout << "...done! " << endl;

   // ---------- EX.1.2.1 Random number generation from other probability distributions ---------------
   // ---------- & Checking the Central Limit Theorem -------------------------------------------------


   
   int n_realizations = 10000; // Number of realizations
   vector<int> sums = {1, 2, 10, 100};


   ofstream outputFileUniform("./CLT_output_files/CLT_uniform_out.dat");
   ofstream outputFileExponential("./CLT_output_files/CLT_exponential_out.dat");
   ofstream outputFileLorentzian("./CLT_output_files/CLT_lorentzian_out.dat");

   // Uniform Distribtution

  for (int i = 0; i < n_realizations; i++) 
  {
    for (int j = 0; j < sums.size(); j++) 
    {
      outputFileUniform << rnd.sum_uniform(sums[j]) << " ";
    }
    outputFileUniform << endl;
  }

   // Exponential Distribtution

  for (int i = 0; i < n_realizations; i++) 
  {
    for (int j = 0; j < sums.size(); j++) 
    {
      outputFileExponential << rnd.sum_exponential(sums[j]) << " ";
    }
    outputFileExponential << endl;
  }

  // Lorentzian Distribtution

  for (int i = 0; i < n_realizations; i++) 
  {
    for (int j = 0; j < sums.size(); j++) 
    {
      outputFileLorentzian << rnd.sum_cauchylorentz(sums[j]) << " ";
    }
    outputFileLorentzian << endl;
  }

   outputFileUniform.close();
   outputFileExponential.close();
   outputFileLorentzian.close();

   
   // ---------- EX.1.3 Simulating Buffon's Experiment ---------------

    int blocks = 100;
    int throws_per_block = 1000000;
    float distance = 0.1;
    float needle_length =0.05;

    double y_random, random_theta;  // Random y coordinate and random angle
    double y1, y2;  // Extremities of the needle 
    int hits_count;

    vector<double> pi_estimate(N, 0);
    vector<double> pi_squared_estimate(N, 0);

    Buffon BuffonExperiment;

    string filename = "./Buffon_output_files/Buffon_out.dat";

    for (int i = 0; i < blocks; i++)
    {
        hits_count = 0;
        for (int j = 0; j < throws_per_block; j++)
        {

            random_theta = rnd.random_theta();
            y_random = rnd.Rannyu(-1,1);

            y1 = y_random + needle_length/2 * sin(random_theta);
            y2 = y_random - needle_length/2 * sin(random_theta);

            double floor_spacing = 0.1;

            // Function to check if the extremities of the needle cross a floor line
            if (BuffonExperiment.crosses_spacing(y1, y2, floor_spacing)) 
            { 
                hits_count +=1;
            } 
        }

        pi_estimate[i] = BuffonExperiment.pi_computation(hits_count, throws_per_block, distance, needle_length);
        pi_squared_estimate[i] = pi_estimate[i] * pi_estimate[i];
    }

   BuffonExperiment.blocked_statistics(pi_estimate, pi_squared_estimate, blocks, filename);

   rnd.SaveSeed();
   return 0;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
