
// ================ EX.2 SEAN GOWEN ===============

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#include "statistics.h"

void blocked_statistics(vector<double> mean, vector<double>  mean_squared, int N_blocks, string filename)
{
   ofstream outfile;
   outfile.open(filename);

   vector<double> sum(N_blocks,0);
   vector<double> sum2(N_blocks,0);
   vector<double> err(N_blocks,0);

   outfile << "Cumulative mean, " << "Cumulative mean squared, " << "error" << endl;

   for (int i=0; i<N_blocks; i++)
   {
      for (int j=0; j<(i+1); j++)
      {
         sum[i] += mean[j];            // cumulative sum of the means
         sum2[i] += mean_squared[j];   // cumulative sum of means squared
      }

      sum[i] /= (i+1); // cumulative mean
      sum2[i] /= (i+1); // cumulative square mean
      err[i] = error(sum, sum2, i); // uncertainty
      outfile << sum[i] << " " << sum2[i] << " " << err[i] << endl;
   }

   outfile.close();
}

/*
void blocked_statistics_random_walk(vector<double> mean_r, vector<double> mean_rsquared, int N_blocks, 
   int L_throws_per_block, string filename)
{
   int N = N_blocks;
   int L = L_throws_per_block;

   ofstream outfile;
   outfile.open(filename);

   vector<double> sum_r(L,0);
   vector<double> sum_rsquared(L,0);
   vector<double> block_error(sum_r, sum_rsquared, 0);

   for(int i=0; i< L; i++)
   {
      sum_r += mean_r[i];
      sum_rsquared += mean_rsquared[i];
   }
}
*/

// error = statistical uncertainty of the mean
float error(vector<double> mean, vector<double> mean_squared, int steps)
{
   if (steps == 0){ return 0; } 
   else {   return sqrt( (mean_squared[steps] - mean[steps]*mean[steps]) / steps);  }   // sqrt( <r^2> - <r>^2 )/ 
}

// Error computation for Random Walk
double compute_error(const vector<double>& block_averages, int n_blocks) 
{
    double mean = 0.0;
    double mean2 = 0.0;

    for (int i = 0; i < n_blocks; i++) 
    {
        mean  += block_averages[i];
        mean2 += block_averages[i] * block_averages[i];
    }

    mean  /= n_blocks;
    mean2 /= n_blocks;

    if (n_blocks <= 1) return 0.0;  // no error with a single block
    return sqrt((mean2 - mean * mean) / (n_blocks - 1));
}
