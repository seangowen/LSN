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



// --------- Routine Functions ------------


void simulate_continuous_random_walk(Random &rnd,
                                     int n_walks_total,
                                     int n_groups,
                                     const string &output_filename)
{
      int walks_per_group = n_walks_total / n_groups;

      ofstream out(output_filename);
      if (!out.is_open()) {
        cerr << "Error: cannot open output file!" << endl;
        return;
      }

      out << "Steps, <r>, Error\n";

      // Loop over number of RW steps
      for (int steps = 0; steps <= n_groups; steps++)
      {
        vector<double> group_means(n_groups, 0.0);

        // ========== GROUP LOOP ==========
        for (int g = 0; g < n_groups; g++)
        {
            double sum_r2 = 0.0;

            for (int w = 0; w < walks_per_group; w++)
            {
                vector<double> position(3);
                continuous_random_walk(position, rnd, steps);

                sum_r2 += continuous_walked_distance(position);
            }

            group_means[g] = sum_r2 / walks_per_group;
        }

        // ========== AVERAGE & ERROR ==========
        double avg_r2 = 0.0;
        double avg_r2_sq = 0.0;

        for (double gm : group_means)
        {
            avg_r2     += gm;
            avg_r2_sq  += gm * gm;
        }

        avg_r2    /= n_groups;
        avg_r2_sq /= n_groups;

        //double var_r2 = avg_r2_sq - avg_r2 * avg_r2;
        //double err_r2 = sqrt(var_r2 / n_groups);

      // observable: r = sqrt(<r²>)
      double avg_r = sqrt(avg_r2);
      double err_r;

      // avoid division by zero for the trivial case
      if (steps == 0) { err_r = 0.0; } 
      else 
      {
          double se_r2 = sqrt( (avg_r2_sq - avg_r2 * avg_r2) / n_groups );
          err_r = se_r2 / (2.0 * avg_r);   // error propagation
      }

        out << steps << "," << avg_r << "," << err_r << "\n";

        cout << "Steps: " << steps
             << " | <r> = " << avg_r
             << " | Error = " << err_r << endl;
      }

      out.close();
}


void simulate_discrete_random_walk(Random &rnd,
                                   int total_walks,
                                   int num_groups,
                                   const string &output_filename)
{
    int walks_per_group = total_walks / num_groups;

    ofstream out(output_filename);
    if (!out.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    out << "Steps taken, Average distance walked, Error\n";

    // Loop over number of steps
    for (int steps = 0; steps <= num_groups; steps++)
    {
        vector<double> group_means(num_groups, 0.0);

        // ========== GROUP LOOP ==========
        for (int g = 0; g < num_groups; g++)
        {
            double sum_d2 = 0.0;

            for (int w = 0; w < walks_per_group; w++)
            {
                vector<int> position = {0, 0, 0};
                random_walk(position, rnd, steps);

                double d2 = walked_distance(position);  // |r|^2
                sum_d2 += d2;
            }

            group_means[g] = sum_d2 / walks_per_group;
        }

        // ========== AVERAGE & ERROR ==========
        double avg_d2 = 0.0;
        double avg_d2_sq = 0.0;

        for (double gm : group_means)
        {
            avg_d2     += gm;
            avg_d2_sq  += gm * gm;
        }

        avg_d2    /= num_groups;
        avg_d2_sq /= num_groups;

        double variance = avg_d2_sq - avg_d2 * avg_d2;
        double se_d2 = sqrt(variance / num_groups);

        // Convert ⟨d²⟩ to ⟨d⟩
        double avg_d = sqrt(avg_d2);
        double err_d = se_d2 / (2.0 * sqrt(avg_d2));

        out << steps << "," << avg_d << "," << err_d << "\n";

        cout << "Steps: " << steps
             << " | <d> = " << avg_d
             << " | Error = " << err_d << endl;
    }

    out.close();
}




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

/* backup
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

      // Divide walks into blocks
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

      // Compute mean and variance across blocks
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

      // Compute sqrt(<d^2>) and propagate error
      double average_distance = sqrt(mean);
      double error_distance = se / (2 * sqrt(mean));

      outputfile_random_walk << n_steps << "," << average_distance << "," << error_distance << endl;

      cout  << "Steps: " << n_steps
            << " | <d> = " << average_distance
            << " | Error = " << error_distance << endl;
   }

*/

   // -------------- Discrete Walk on a cubic lattice ------------

   simulate_discrete_random_walk(
     rnd,
     10000,                                  // total walks
     100,                                    // number of groups
     "output_files/ex2_2_discrete_random_walk.dat"
   );

   // -------------- Continuous Walk on a sphere -----------------

   simulate_continuous_random_walk(
     rnd,
     10000,                                  // n_walks_total
     100,                                    // n_groups
     "output_files/ex2_2_continuous_random_walk.dat"  // output file
   );


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
