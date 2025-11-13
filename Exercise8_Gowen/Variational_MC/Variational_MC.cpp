#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Variational_MC.h"
#include "random.h"

using namespace std;



int main() {
  Initialize();
  for (int iblk = 1; iblk <= n_blks; ++iblk) {
    Reset(iblk);
    for (int istep = 1; istep < n_steps; ++istep) {
      Move();
      if (istep % iprint == 0)
        PrintPos(iblk); // For histogram
      if (istep % imeasure == 0)
        Measure();
      Accumulate();
    }
    Average(iblk); // print block and cumulative averages
    Report(iblk); // print to terminal
  }
}

void Initialize() {

  // Initialize RNG
  ifstream input("Primes");
  input >> p1 >> p2;
  input.close();

  input.open("seed.in");
  string property;
  input >> property;
  if (property == "RANDOMSEED") {
    input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
  }
  rnd.SetRandom(seed, p1, p2);
  input.close();

  ReadInput(); // read from input.dat
  Welcome(); // message to terminal

  x = x0; // starting position

  iu = 0; // walker index for energy
  n_props = 1;

  // create empty output files
  ofstream clear("positions.dat");
  clear.close();
  clear.open("energy.dat");
  clear.close();
}

void Reset(int iblk) { // reset accumulators

  if (iblk == 1) {
    for (int i = 0; i < n_props; ++i) {
      glob_av[i] = 0;
      glob_av2[i] = 0;
    }
  }

  for (int i = 0; i < n_props; ++i) {
    blk_av[i] = 0;
  }
  blk_norm = 0;
  attempted = 0;
  accepted = 0;
}

void Move() { // Metropolis
  double alpha;
  // x_new = x + rnd.Rannyu(-delta, delta);   // new position
  x_new = rnd.Gauss(x, delta);
  alpha = pdf(x_new, mu, sigma) / pdf(x, mu, sigma); // acceptance probability
  attempted++;
  if (rnd.Rannyu() < alpha) { // accept or reject
    x = x_new;
    accepted++;
  }
}

void PrintPos(int iblk) {
  ofstream pos;
  pos.open("positions.dat", ios::app);
  pos << iblk << " " << x << endl;
  pos.close();
}

void Measure() { // collect local energy
  walker[iu] = eloc(x, mu, sigma);
}

void Accumulate() { // collect sums to average
  for (int i = 0; i < n_props; ++i) {
    blk_av[i] = blk_av[i] + walker[i];
  }
  blk_norm = blk_norm + 1.0;
}

void Average(int iblk) { // Calculate and print
  ofstream Ene;
  int pc = 6;

  Ene.open("energy.dat", ios::app);
  stima_u = blk_av[iu] / blk_norm; // Average Energy
  glob_av[iu] += stima_u;
  glob_av2[iu] += stima_u * stima_u;
  err_u = Error(glob_av[iu], glob_av2[iu], iblk);

  Ene << iblk << " ";
  Ene << setprecision(pc) << stima_u << " ";
  Ene << setprecision(pc) << glob_av[iu] / (double)iblk << " ";
  Ene << setprecision(pc) << err_u << endl;
  Ene.close();
}

void Report(int iblk) { // print to terminal
  cout << "Block number " << iblk << endl;
  cout << "Acceptance rate : " << (float)accepted / attempted << endl;
  cout << "Estimate : " << stima_u << endl;
  cout << "Average : " << glob_av[iu] / (double)iblk << endl;
  cout << "-------------------------------" << endl;
}

double Error(double sum, double sum2, int iblk) {
  if (iblk == 1)
    return 0.0;
  else
    return sqrt((sum2 / (double)iblk - pow(sum / (double)iblk, 2)) / (double)(iblk - 1));
}

// ====== PHYSICS ========

double wavefunction(double x, double mu, double sigma) {
  double a, b;
  // non normalizzata
  a = exp(-(x - mu)*(x - mu) / (2 * sigma * sigma));
  b = exp(-(x + mu)*(x + mu) / (2 * sigma * sigma));
  return (a + b);
}

double pdf(double x, double mu, double sigma) {
  double a, b, norm;
  // modulo quadro normalizzato

  a = exp(-(x - mu) * (x - mu) / (2 * sigma * sigma));
  b = exp(-(x + mu) * (x + mu) / (2 * sigma * sigma));

  norm = exp(mu * mu / (sigma * sigma));
  norm /= ((1 + norm) * 2 * sigma * sqrt(M_PI));
  return (a + b) * (a + b) / norm;
}

double eloc(double x, double mu, double sigma) {
  // (H psi) / psi
  double a, b, xmm, xpm, s2, s4;
  double kinetic, potential, energy;

  xmm = (x - mu) * (x - mu);
  xpm = (x + mu) * (x + mu);
  s2 = sigma * sigma;
  s4 = sigma * sigma * sigma * sigma;

  a = exp(-xmm / (2 * s2));
  b = exp(-xpm / (2 * s2));

  // - psi''
  kinetic = -(a * xmm + b * xpm) / s4 + (a + b) / s2;
  kinetic /= 2.0;

  potential = (x * x - 2.5) * x * x;
  // potential = 0.5*x*x;

  // (H psi ) / psi
  energy = kinetic + potential * wavefunction(x, mu, sigma);
  energy /= wavefunction(x, mu, sigma);

  return energy;
}

// ==== ====

void Welcome() { // Welcome message
  cout << "Program for Variational Monte Carlo." << endl;
  cout << n_blks << " blocks and ";
  cout << n_steps << " steps" << endl;
  cout << "Mu : " << mu << ", Sigma : " << sigma << endl;
  cout << endl;
}

void ReadInput() { // Read from input.dat
  ifstream input("input.dat");
  input >> x0;
  input >> delta;
  input >> n_steps;
  input >> n_blks;
  input >> mu;
  input >> sigma;
  input >> iprint;
  input >> imeasure;
  input.close();
}
