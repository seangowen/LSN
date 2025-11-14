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
#include <cmath>
#include "random.h"
#include "GeometricBrownianMotion.h"
#include "statistics.h"

using namespace std;

int main(int argc, char* argv[]) {

    Random rnd;
    int seed[4];
    int p1, p2;

    ifstream Primes("Primes");
    if (Primes.is_open()) {
        Primes >> p1 >> p2;
        Primes.close();
    } else {
        cerr << "PROBLEM: Unable to open Primes" << endl;
        return 1;
    }

    ifstream input("seed.in");
    string property;
    if (input.is_open()) {
        while (!input.eof()) {
            input >> property;
            if (property == "RANDOMSEED") {
                input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                rnd.SetRandom(seed, p1, p2);
            }
        }
        input.close();
    } else {
        cerr << "PROBLEM: Unable to open seed.in" << endl;
        return 1;
    }

    // ---------------- Parameters -------------------

    const double S0 = 100.0;    // Initial asset price
    const double K = 100.0;     // Strike price
    const double T = 1.0;       // Maturity
    const double mu = 0.1;      // Risk-free interest rate
    const double sigma = 0.25;  // Volatility

    const int M = 100000;       // Total Monte Carlo throws
    const int N = 100;          // Number of blocks
    const int timesteps = 100;  // For discretized GBM
    const double dt = T / timesteps;


    vector<double> call_ave(N, 0.0);
    vector<double> call_av2(N, 0.0);
    vector<double> put_ave(N, 0.0);
    vector<double> put_av2(N, 0.0);

    // ------------------- DIRECT SAMPLING -------------------

    cout << "Starting direct sampling ..." << endl;

    GeometricBrownianMotion gbm(S0, mu, sigma);

    for (int i = 0; i < N; i++) {
        call_ave[i] = 0.0;
        put_ave[i] = 0.0;

        for (int j = 0; j < M; j++) {
            double S_T = gbm.asset_terminal_price(T, rnd);

            double diff_call = S_T - K;
            double diff_put  = K - S_T;

            call_ave[i] += exp(-mu*T) * max(0.0, diff_call);
            put_ave[i]  += exp(-mu*T) * max(0.0, diff_put);
        }

        call_ave[i] /= M;
        call_av2[i] = call_ave[i] * call_ave[i];

        put_ave[i] /= M;
        put_av2[i] = put_ave[i] * put_ave[i];
    }

    string datafile = "./output_files/call_option_direct_sampling.dat";
    blocked_statistics(call_ave, call_av2, N, datafile);

    datafile = "./output_files/put_option_direct_sampling.dat";
    blocked_statistics(put_ave, put_av2, N, datafile);

    // ------------------- DISCRETIZED PATH SAMPLING -------------------

    cout << "Starting discretized path sampling (might take longer) ..." << endl;

    fill(call_ave.begin(), call_ave.end(), 0.0);
    fill(call_av2.begin(), call_av2.end(), 0.0);
    fill(put_ave.begin(), put_ave.end(), 0.0);
    fill(put_av2.begin(), put_av2.end(), 0.0);

    // NOTE: in the end, I found it simpler to compute S_T
    // directly inside the loop

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            double S_T = S0;

            // Simulating the entire path
            for (int k = 0; k < timesteps; k++) {
                double Z = rnd.Gauss(0.0, 1.0);
                S_T = S_T * exp((mu - 0.5*sigma*sigma)*dt + sigma*sqrt(dt)*Z);
            }

            double diff_call = S_T - K;
            double diff_put  = K - S_T;

            call_ave[i] += exp(-mu*T) * max(0.0, diff_call);
            put_ave[i]  += exp(-mu*T) * max(0.0, diff_put);
        }

        call_ave[i] /= M;
        call_av2[i] = call_ave[i] * call_ave[i];

        put_ave[i] /= M;
        put_av2[i] = put_ave[i] * put_ave[i];
    }

    datafile = "./output_files/call_option_path_sampling.dat";
    blocked_statistics(call_ave, call_av2, N, datafile);

    datafile = "./output_files/put_option_path_sampling.dat";
    blocked_statistics(put_ave, put_av2, N, datafile);

    rnd.SaveSeed();
    cout << "Simulation completed!" << endl;

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
