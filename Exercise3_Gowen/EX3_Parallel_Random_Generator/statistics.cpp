
// ================ EX.3 SEAN GOWEN ===============

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#include "statistics.h"

void blocked_statistics(vector<double> average, vector<double>  average2, int N, string filename)
{
   ofstream outfile;
   outfile.open(filename);

   vector<double> sum(N,0);
   vector<double> sum2(N,0);
   vector<double> err(N,0);

   outfile << "Cumulative average, Cumulative square average, error" << endl;
   for (int i=0; i<N; i++)
   {
      for (int j=0; j<(i+1); j++)
      {
         sum[i] += average[j];      // cumulative sum of averages
         sum2[i] += average2[j];    // cumulative sum of squared averages
      }

      sum[i] /= (i+1); // cumulative average
      sum2[i] /= (i+1); // cumulative square average
      err[i] = error(sum, sum2, i); // uncertainty
      outfile << sum[i] << "," << sum2[i] << "," << err[i] << endl;
   }

   outfile.close();
}

float error(vector<double> average, vector<double> average2, int n)
{
   if (n == 0)
   {
      return 0;
   } 
   else 
   {
      return sqrt( (average2[n] - average[n]*average[n]) / n);
   }
}

