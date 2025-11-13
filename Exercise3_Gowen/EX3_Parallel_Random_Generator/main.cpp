/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/


// =============== MAIN.CPP TEMPLATE SEAN GOWEN =================


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "math.h"
#include "random.h"
#include "GeometricBrownianMotion.h"
#include "statistics.h"

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

   for(int i=0; i<20; i++){
      cout << rnd.Rannyu() << endl;
   }    


   // ------------------- EX.3 -------------------------


   // Parameters

   double S0 = 100; // Initial Asset Price
   double S_T = 0; // Terminal Asser Price
   double mu = 0.1; // Risk Free Interest Rate
   double sigma = 0.25; // Volatility
   double K = 100; // Strike Price
   double T = 1.0; // Delivery Time


   // ---------------------------------------------------
   // Direct Sampling of the Final Asset Price


   int M = 100000; // TOTAL Number of throws
   int N = 100; // Number of blocks
   // N.B. there are L= M/N throws per block

   double diff;

   vector<double> call_ave(N,0);
   vector<double> call_av2(N,0);
   vector<double> put_ave(N,0);
   vector<double> put_av2(N,0);


   for (int i=0; i < N; i++ )
   {
      S0 = 100;

      for (int j=0; j < M; j++)
      {
         S_T = gbm.asset_terminal_price(T, rnd);

         diff = S_T-K;
         call_ave[i] += exp(-mu*T)*max(0., diff); // Call Price

         diff = K-S_T;
         put_ave[i] += exp(-mu*T)*max(0., diff); // Put Price
      }

      call_ave[i] /= M;
      call_av2[i] = call_ave[i]*call_ave[i];

      put_ave[i] /= M;
      put_av2[i] = put_ave[i]*put_ave[i];
   }


   // Cumulative blocked statistics

   string datafile = "";

   datafile = "./output_files/call_option_direct_sampling.dat";
   blocked_statistics(call_ave, call_av2, N, datafile);

   datafile = "./output_files/put_option_direct_sampling.dat";
   blocked_statistics(put_ave, put_av2, N, datafile);


   // ---------------------------------------------------
   // Discretized Sampling of the Final Asset Price

   
   // 3.1.2 - Path Sampling of GBM ------> Check Code Snippet Below

   M = 100000;
   int timesteps = 100;
   float t = T / timesteps;

   fill(call_ave.begin(), call_ave.end(), 0);
   fill(call_av2.begin(), call_av2.end(), 0);
   fill(put_ave.begin(), put_ave.end(), 0);
   fill(put_av2.begin(), put_av2.end(), 0);

   GeometricBrownianMotion gbm(S0, mu, sigma);

   for (int i = 0; i < N; i++) // Blocks
   {
      for (int j = 0; j < M; j++) // Sampling the future price
      { 
         S_T = 100;
         for (int k = 0; k < timesteps; k++) // Path simulation
         { 
            S_t = gbm(S_T, t, r, sigma, rnd);
         }

         diff = S_T-K;
         call_ave[i] += exp(-r*T)*max(0., diff); // Call Price

         diff = K-S_T;
         put_ave[i] += exp(-r*T)*max(0., diff); // Put Price

      }

      call_ave[i] /= M;
      call_av2[i] = call_ave[i]*call_ave[i];

      put_ave[i] /= M;
      put_av2[i] = put_ave[i]*put_ave[i];
   }

  // Statistics

  datafile = "./output_files/call_option_path_sampling.dat";
  blocked_statistics(call_ave, call_av2, N, datafile);

  datafile = "./output_files/put_option_path_sampling.dat";
  blocked_statistics(put_ave, put_av2, N, datafile);


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
