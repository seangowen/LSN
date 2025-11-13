/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/


// =============== EX.2 SEAN GOWEN =================


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "math.h"
#include "random.h"
#include "statistics.h"
#include "RandomWalk.h"

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

   cout << "Printing the first 20 random numbers for debugging" << endl;
   for(int i=0; i<20; i++){
      cout << rnd.Rannyu() << endl;
   }


   // =================== Ex. 2.1 1D Monte Carlo Integral ===================

   // ------------------- 2.1.1 1D Integral via Monte Carlo (MC) ------------

   int M = 1000000;  // Throws
   int N = 100;      // Blocks
   int L = M/N;      // Throws per block
   double r;

   vector<double> mean_r(N,0);           // Mean values of MC integral values computed for each Block
   vector<double> mean_rsquared(N,0);   // Mean squared

   // MC Integration

   for (int i = 0; i < N; i++) 
   {
      for (int j = 0; j < L; j++) 
      {
         r = rnd.Rannyu();
         mean_r[i] += (M_PI/2) * cos(M_PI * 0.5 * r); // Integrand
      }

      mean_r[i] /= L;                              // <r>
      mean_rsquared[i] = (mean_r[i]*mean_r[i]);    // <r^2>
   }

   // Blocked Statistics

   string outputfile = "output_files/ex2_1_monte_carlo_output.dat";
   blocked_statistics(mean_r, mean_rsquared, N, outputfile);


   // ------------------- 2.1.2 1D Integral via Monte Carlo Importance Sampling ---

   // IS = Importance Sampling
   vector<double> mean_IS(N,0);     // Averages of MC integral values computed for each Block
   vector<double> mean_ISsquared(N,0);    // Averages squared

   for (int i = 0; i < N; i++) 
   {
      for (int j = 0; j < L; j++) 
      {
         r = rnd.ImportanceSampling();
         mean_IS[i] +=-(M_PI/4)*cos((M_PI/2)*r) / (r - 1); // Integrand
      }

      mean_IS[i] /= L;                                // <IS>
      mean_ISsquared[i] = (mean_IS[i]*mean_IS[i]);    // <IS^2>
   }

   // Blocked Statistics

   outputfile = "output_files/ex2_2_monte_carlo_output_importance_sampling.dat";
   blocked_statistics(mean_IS, mean_ISsquared, N, outputfile);


   // =================== Ex. 2.2 Random Walk ===================


   int n_rw = 10000;       // total number of random walks
   int n_blocks = 100;     // number of blocks
   int walks_per_block = n_rw / n_blocks; // walks per block

   vector<double> average_modulus_square_distance_per_block(n_blocks, 0.0);
   vector<double> error(n_blocks, 0.0);

   ofstream outputfile_random_walk("output_files/ex2_2_discrete_random_walk.dat");
   if (!outputfile_random_walk.is_open()) 
   {
      cerr << "Error opening file!" << endl;
      return 1;
   }
   
   outputfile_random_walk << "Steps taken, Average distance walked, Error\n";

   // Loop over number of steps
   for (int n_steps = 0; n_steps <= n_blocks; n_steps++) 
   {

      // 1️⃣ Divide walks into blocks
      vector<double> block_averages(n_blocks, 0.0);

      for (int i = 0; i < n_blocks; i++) 
      {
         double sum_block = 0.0;

         for (int j = 0; j < walks_per_block; j++) 
         {
            vector<int> position = {0, 0, 0};
            random_walk(position, rnd, n_steps);

            double d2 = walked_distance(position); // d^2
            sum_block += d2;
         }

         block_averages[i] = sum_block / walks_per_block; // average per block
      }

      // 2️⃣ Compute mean and variance across blocks
      double mean = 0.0;
      double mean2 = 0.0;

      for (int i = 0; i < n_blocks; i++) 
      {
         mean += block_averages[i];
         mean2 += block_averages[i] * block_averages[i];
      }

      mean /= n_blocks;
      mean2 /= n_blocks;

      double variance = mean2 - mean*mean;
      double se = sqrt(variance / n_blocks); // standard error of mean

      // 3️⃣ Compute sqrt(<d^2>) and propagate error
      double average_distance = sqrt(mean);
      double error_distance = se / (2 * sqrt(mean));

      // 4️⃣ Write to file
      outputfile_random_walk << n_steps << "," << average_distance << "," << error_distance << endl;

      cout  << "Steps: " << n_steps
            << " | <d> = " << average_distance
            << " | Error = " << error_distance << endl;
   }



   // ------- BACKUP -------
   /*
   int n_rw = 10000;       // number of random walks
   int n_blocks = 100;   // number of blocks

   vector<double> average_modulus_square_distance_per_block(n_blocks+1, 0.0);
   //vector<double> average_walked_distance_per_block(n_blocks+1, 0.0);
   //vector<double> average_squared_walked_distance_per_block(n_blocks+1, 0.0);

   // Generic average values vectors to be used for error computations
   vector<double> average_value(n_blocks+1, 0.0);           // <A>
   vector<double> average_squared_value(n_blocks+1, 0.0);   // <A^2>
   vector<double> average_value_squared(n_blocks+1, 0.0);   // <A>^2

   vector<double> error(n_blocks+1, 0.0);

   std::ofstream outputfile_random_walk("output_files/ex2_2_discrete_random_walk.dat");

   if (!outputfile_random_walk.is_open()) 
   {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
   }

   outputfile_random_walk << "Steps taken, Average distance walked , error\n";

   for (int i = 0; i <= n_blocks; i++)  // allow up to 100 steps
   {
      double sum_d_squared = 0.0;
      double sum_d_squared_squared = 0.0;

      for (int j = 0; j < n_rw; j++) 
      {
         int n_steps = i;

         vector<int> position = {0,0,0};  // reset per walk
         random_walk(position, rnd, n_steps);

         double d_squared = walked_distance(position);
         sum_d_squared  += d_squared;
         sum_d_squared_squared += d_squared*d_squared;
      }

      average_modulus_square_distance_per_block[i] = sum_d_squared / n_rw;

      average_value[i] = average_modulus_square_distance_per_block[i];
      average_squared_value[i] = sum_d_squared_squared / n_rw;
      average_value_squared[i] = average_value[i] * average_value[i];

      //error[i] = sqrt(average_squared_value[i] - average_value_squared[i]);    // √ (<A^2> - <A>^2)

      error[i] = sqrt( (average_squared_value[i] - average_value[i]*average_value[i]) / n_rw );
      
      double average_distance_walked = sqrt(average_modulus_square_distance_per_block[i]);

      cout << "Block " << i << " -- steps taken: " << i << endl;
      cout << "Average distance walked = " << average_distance_walked << endl;
      cout << "---------------------------------------------" << endl;

      outputfile_random_walk << i << ","  << average_distance_walked << ","
                                          << error[i] << endl;

   }
   */

   /*
   // -------------- !!! KEEP FOR LATER COMPARAISON !!! Discrete Walk on a lattice --------------

   int n_rw_experiments = 10000; // Number of random walks experiments
   int n_max_steps = 100;        // Max number of steps

   // Resetting average vectors to zero

   fill(mean_r.begin(), mean_r.end(), 0);
   fill(mean_rsquared.begin(), mean_rsquared.end(), 0);

   vector<int> position(3); // a 3D vector for position

   for (int i=0; i < n_max_steps; i++)
   {

      for (int j=0; j < n_rw_experiments; j++)
      {
         // For each new experiment, setting the initial position at the origin

         position[0] = 0;
         position[1] = 0;
         position[2] = 0;

         random_walk(position, rnd, i+1); // updating the position at each random step

         mean_r[i] += walked_distance(position);
      }

      mean_r[i] /= n_rw_experiments;
      mean_rsquared[i] = mean_r[i]*mean_r[i]; // --> WRONG ??

      //average2_correct[i] += pow(walked_distance(position), 2); // accumulate r^2
   }

   outputfile = "output_files/ex2_2_discrete_random_walk.dat";
   blocked_statistics(mean_r, mean_rsquared, n_max_steps, outputfile);

   string outputfile_correct = "output_files/ex2_2_discrete_random_walk_correct.dat";
   //blocked_statistics(average, average2_correct, n_max_steps, outputfile_correct);

   */



   // -------------- Continuous Walk on a sphere -----------------


   /*
   n_rw_experiments = 10000; // Number of random walks experiments
   n_max_steps = 100;        // Max number of steps

   // Resetting average vectors to zero

   fill(mean_r.begin(), mean_r.end(), 0);
   fill(mean_rsquared.begin(), mean_rsquared.end(), 0);

   vector<double> sphere_position(3); // a 3D vector for position


   for (int i = 0; i < n_max_steps; i++)
   {

      for (int j = 0; j < n_rw_experiments; j++) 
      {

         sphere_position[0] = 0;
         sphere_position[1] = 0;
         sphere_position[2] = 0;

         continuous_random_walk(sphere_position, rnd, i+1);
         mean_r[i] += continuous_walked_distance(sphere_position);
      }

      mean_r[i] /= n_rw_experiments;
      mean_rsquared[i] = mean_r[i]*mean_r[i];
   }

   outputfile = "output_files/ex2_2_continuous_random_walk.dat";
   blocked_statistics(mean_r, mean_rsquared, n_max_steps, outputfile);

   */

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
