#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "random.h"
#include "math.h"
#include "MonteCarloIntegration.h"

using namespace std;

MonteCarloIntegration :: MonteCarloIntegration(){}
MonteCarloIntegration :: ~MonteCarloIntegration(){}


// Ex. 1.1

void MonteCarloIntegration :: firstIntegral(int M, int N, int L, Random rnd, std::vector<double> ave, std::vector<double> ave2, std::vector<double> sumProg, std::vector<double> sumProg2)
{
   std::vector<double> errProg(N,0);

	ofstream outputFile("./Monte_Carlo_output_files/MonteCarlo1_out.dat");


   for (int i=0; i<N; i++)
   {
      for (int j=0; j<L; j++)
      {
         ave[i] += rnd.Rannyu();
      }

      ave[i] /= L;
      ave2[i] = ave[i]*ave[i];
   }

   for (int i=0; i<N; i++)
   {
      for (int j=0; j<(i+1); j++)
      {
         sumProg[i] += ave[j];
         sumProg2[i] += ave2[j];
      }

      sumProg[i] /= (i+1);
      
      sumProg2[i] /= (i+1);
      errProg[i] = error(sumProg, sumProg2, i);

      outputFile << sumProg[i] << " " << errProg[i] << endl;
   }

   outputFile.close();
}


void MonteCarloIntegration :: secondIntegral(int M, int N, int L, Random rnd, std::vector<double> ave, std::vector<double> ave2, std::vector<double> sumProg, std::vector<double> sumProg2)
{
   std::vector<double> errProg(N,0);

   ofstream outputFile("./Monte_Carlo_output_files/MonteCarlo2_out.dat");

   double r;

   for (int i=0; i<N; i++)
   {
      for (int j=0; j<L; j++)
      {
         r = rnd.Rannyu();
         ave[i] += (r-0.5)*(r-0.5);
      }

      ave[i] /= L;
      ave2[i] = ave[i]*ave[i];
   }

   for (int i=0; i<N; i++)
   {
      for (int j=0; j<(i+1); j++)
      {
         sumProg[i] += ave[j];
         sumProg2[i] += ave2[j];
      }

      sumProg[i] /= (i+1);
      sumProg2[i] /= (i+1);

      errProg[i] = error(sumProg, sumProg2, i);

      outputFile << sumProg[i] << " " << errProg[i] << endl;
   }

   outputFile.close();
}


// Ex. 2.1 work in progress


void MonteCarloIntegration :: thirdIntegral(int M, int N, int L, Random rnd, std::vector<double> ave, std::vector<double> ave2, std::vector<double> sumProg, std::vector<double> sumProg2)
{
   std::vector<double> errProg(N,0);

   ofstream outputFile("./Monte_Carlo_output_files/MonteCarlo3_out.dat");

   double r;

   for (int i=0; i<N; i++)
   {
      for (int j=0; j<L; j++)
      {
         r = rnd.Rannyu();
         ave[i] += (M_PI/2.0) * cos(M_PI*(r/2.0)) - 1 ;
      }

      ave[i] /= L;
      ave2[i] = ave[i]*ave[i];
   }

   for (int i=0; i<N; i++)
   {
      for (int j=0; j<(i+1); j++)
      {
         sumProg[i] += ave[j];
         sumProg2[i] += ave2[j];
      }

      sumProg[i] /= (i+1);
      sumProg2[i] /= (i+1);

      errProg[i] = error(sumProg, sumProg2, i);

      outputFile << sumProg[i] << " " << errProg[i] << endl;
   }

   outputFile.close();
}



float MonteCarloIntegration :: error(std::vector<double> sumProg, std::vector<double> sumProg2, int i)
{
   if (i==0) return 0;
   else 
   {
      return sqrt((sumProg2[i]-sumProg[i]*sumProg[i])/i);
   }
}





