#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "random.h"
#include "Buffon.h"

using namespace std;

Buffon:: Buffon(){}
Buffon:: ~Buffon(){}


void Buffon :: blocked_statistics(vector<double>  average, vector<double>  average_squared, int N, string filename)
{
   vector<double> sum(N,0);
   vector<double> sum2(N,0);
   vector<double> err(N,0);

   ofstream outputfile(filename);

   for (int i=0; i<N; i++)
   {
      for (int j=0; j<(i+1); j++)
      {
         sum[i] += average[j]; // cumulative sum of averages
         sum2[i] += average_squared[j]; // sum of square averages
      }

      sum[i] /= (i+1); // cumulative average
      sum2[i] /= (i+1); // cumulative square average
      err[i] = error(sum, sum2, i); // uncertainty

      outputfile << sum[i] << " " << sum2[i] << " " << err[i] << endl;
   }

   outputfile.close();
}

bool Buffon :: crosses_spacing(double y1, double y2, double spacing) 
{
   double minY = std::min(y1, y2);
   double maxY = std::max(y1, y2);

   // Find the smallest n such that n * spacing >= minY
   int n_min = std::ceil(minY / spacing);
   // Find the largest n such that n * spacing <= maxY
   int n_max = std::floor(maxY / spacing);

   return n_min <= n_max; // True if there exists at least one n in this range
}

float Buffon :: error(vector<double> average, vector<double> average_squared, int n)
{
   if (n == 0) return 0;
   else 
   {
      return sqrt( (average_squared[n] - average[n]*average[n]) / n);
   }
}

float Buffon :: pi_computation(int hits, int n_throws, float d, float length)
{
   if (hits==0) return 0;
   else 
   {
      return (2*length*n_throws) / (hits * d);
   }
}



