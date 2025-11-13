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
#include <ostream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>
#include "Monte_Carlo_ISING_1D.h"

using namespace std;

// ----------------- Small helpers added --------------------
void RunSimulation();
string GetOutputPrefix();

// ---------------- MAIN PROGRAM ----------------------------

int main()
{
  // Temperatures to run automatically
  vector<double> temp_values = {0.2, 0.5, 0.7, 1.0, 1.2, 1.5, 1.7, 2.0};

  // --- Special case: T = infinity ---
  cout << "===============================" << endl;
  cout << "Running special case: T = inf" << endl;
  cout << "===============================" << endl;
  Input("T=inf");                // this will initialize RNG and read input.dat etc.
  // Ensure we treat temperature symbolically as infinite:
  temp  = numeric_limits<double>::infinity();
  beta  = 0.0;
  Equilibrate_system(1000);
  RunSimulation();

  // --- Special case: T = 0 ---
  cout << "===============================" << endl;
  cout << "Running special case: T = 0" << endl;
  cout << "===============================" << endl;
  Input("T=0");
  temp  = 0.0;
  beta  = numeric_limits<double>::infinity();
  Equilibrate_system(1000);
  RunSimulation();

  // --- Loop over the requested temperatures ---
  for (double T : temp_values) {
    cout << "===============================" << endl;
    cout << "Running simulation at T = " << T << endl;
    cout << "===============================" << endl;

    // Call Input to initialize RNG, read parameters (nspin, J, h, metro, nblk, nstep, restart,...)
    // Use initial_condition "T=inf" so that if restart=="true" we generate random config (see your function)
    Input("T=inf");

    // Set the actual temperature and beta now:
    temp = T;
    if (temp > 0.0 && isfinite(temp)) beta = 1.0 / temp;
    else if (temp == 0.0) beta = numeric_limits<double>::infinity();
    else beta = 0.0;

    // Equilibrate and run
    Equilibrate_system(1000);
    RunSimulation();
  }

  cout << "All simulations complete!" << endl;
  return 0;
}

// ---------------- ROUTINE RUNNER ----------------------------

void RunSimulation() 
{
    cout << "------------------- Simulation -------------------" << endl;

    for (int iblk = 1; iblk <= nblk; ++iblk) 
    {
        Reset(iblk);

        for (int istep = 1; istep <= nstep; ++istep) 
        {
            Move(metro);
            Measure();
            Accumulate();
        }

        Averages(iblk);

        // === Progress bar ===
        double progress = double(iblk) / double(nblk);
        int barWidth = 50;
        cout << "\rBlocks: [";
        int pos = static_cast<int>(barWidth * progress);
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) cout << "=";
            else if (i == pos) cout << ">";
            else cout << ".";
        }
        cout << "] " << iblk << "/" << nblk << flush;
    }

    cout << endl << "Simulation complete. Saving final configuration..." << endl;
    ConfFinal();
}


// ---------------- FILE NAMING HELPER -------------------------

string GetOutputPrefix() 
{
    string algo_name = (metro == 1) ? "Metropolis" : "Gibbs";
    string label;

    if (isinf(temp)) {
        label = "Tinf";
    }
    else if (temp == 0.0) 
    {
        label = "T0.0";
    }
    else 
    {
      ostringstream Tstr;
      Tstr << fixed << setprecision(1) << temp;  // one decimal place always shown
      label = "T" + Tstr.str();
    }

    return "./Output_data/output_" + label + "_" + algo_name;
}



// ---------------- ORIGINAL FUNCTIONS (unchanged logic) --------------------

void Input(string initial_condition)
{
  ifstream ReadInput;

  cout << "Classic 1D Ising model             " << endl;
  cout << "Monte Carlo simulation             " << endl << endl;
  cout << "Nearest neighbour interaction      " << endl << endl;
  cout << "Boltzmann weight exp(- beta * H ), beta = 1/T " << endl << endl;
  cout << "The program uses k_B=1 and mu_B=1 units " << endl;

  //Read seed for random numbers
  int p1, p2;
  ifstream Primes("Primes");
  Primes >> p1 >> p2 ;
  Primes.close();

  ifstream input("seed.in");
  input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
  rnd.SetRandom(seed,p1,p2);
  input.close();
  
  //Read input informations
  ReadInput.open("input.dat");

  ReadInput >> temp;
  beta = 1.0/temp;
  cout << "Temperature = " << temp << endl;

  ReadInput >> nspin;
  cout << "Number of spins = " << nspin << endl;

  ReadInput >> J;
  cout << "Exchange interaction = " << J << endl;

  ReadInput >> h;
  cout << "External field (h) = " << h << endl << endl;
    
  ReadInput >> metro;     // if=1 Metropolis else Gibbs

  ReadInput >> nblk;

  ReadInput >> nstep;

  ReadInput >> restart;   // Lets the program start from a new configuration or carry on from an existing one
                          // If "true", initialize spins from config.0 (new start);
                          // if "false", load spins from config.final (continue previous run)

  if(metro==1) cout << "The program performs Metropolis moves" << endl;
  else cout << "The program performs Gibbs moves" << endl;
  cout << "Number of blocks = " << nblk << endl;
  cout << "Number of steps in one block = " << nstep << endl << endl;

  ReadInput >> varyTemp;

  ReadInput.close();


  //Prepare arrays for measurements
  iu = 0; //Energy
  ic = 1; //Heat capacity
  im = 2; //Magnetization
  ix = 3; //Magnetic susceptibility
 
  n_props = 4; //Number of observables

  //initial configuration
  /*
  for (int i=0; i<nspin; ++i)
  {
    if(rnd.Rannyu() >= 0.5) s[i] = 1;
    else s[i] = -1;
  }
  */

  if(restart=="true")
  {
    random_initial_system_config(initial_condition, "config.0");
    ifstream in;
    in.open("config.0");
    for(int i=0; i<nspin; i++) in >> s[i];
    in.close();
  }
  else
  {
    ifstream in;
    in.open("config.final");
    for(int i=0; i<nspin; i++) in >> s[i];
    in.close();
  }
  
  //Evaluate energy etc. of the initial configuration
  Measure();

  //Print initial values for the potential energy and virial
  cout << "Initial energy = " << walker[iu]/(double)nspin << endl;
}


// -------------- EQUILIBRATION FUNCTION --------------------------

void Equilibrate_system(int equi_steps)
{
  cout << "------------------- Equilibration ------------------- " << endl; 
  cout << "Performing: " << equi_steps << " equilibration steps. " << endl;
  cout << "Saving equilibrated configuration into \"config.final\"" << endl;
  for(int i=0; i<equi_steps; i++) Move(metro);          
  set_restart("true","false");                          
  ConfFinal();
  return;
}

void set_restart(string current, string new_val)
{
  string strReplace = current;
  string strNew = new_val;
  ifstream filein("input.dat");
  ofstream fileout("input.temp");

  while(!filein.eof())
  {
    string temp;
    filein >> temp;
    if(temp==current) fileout << new_val << endl;
    else fileout << temp << endl;
  }
  rename("input.temp", "input.dat");
}


// -------------- PREPARING RANDOM INITIAL CONFIGURATION config.0 -----------

// If T = 0: set all spins to +1
// Else if T -> infinity: randomly set the spins to ±1
void random_initial_system_config(string initial_condition, string filename){
  ofstream out;
  out.open(filename);

  if(initial_condition == "T=0"){
    cout << "Initial condition is T=0" << endl;
    for(int i=0; i<nspin; ++i){
       s[i] = 1;
       out << s[i] << endl;
    }
  }
  else{
    for(int i=0; i<nspin; ++i){
      if(rnd.Rannyu() >= 0.5) s[i] = 1;
      else s[i] = -1;
      out << s[i] << endl;
    }
  }
  out.close();
  return;
}

// -----------------------------------------------------------------


void Move(int metro)
{
  int o;
  double deltaE, r, alpha;
  double p, energy_old, energy_new, sm;
  double energy_up, energy_down;

  for(int i=0; i<nspin; ++i)
  {
    //Select randomly a particle (for C++ syntax, 0 <= o <= nspin-1)
    o = (int)(rnd.Rannyu()*nspin);


    if(metro==1) //Metropolis
    {
      deltaE = -2 * Boltzmann(s[o], o);
      alpha = min(1., exp(-beta*deltaE));
      attempted += 1;
      r = rnd.Rannyu();
      if (r <= alpha) 
      {
        s[o] *= -1;
        accepted += 1;
      }
    }
    else //Gibbs sampling
    {
      deltaE = -2 * Boltzmann(s[o], o) * s[o]; // s[o] * s[o] = 1
      p = 1./(1 + exp(-beta * deltaE)); // probability to have p( s_o = 1 | {s_j : j=/=o})
      attempted += 1;
      accepted += 1;
      // Decide what value should assume spin s_o
      r = rnd.Rannyu();
      if (r <= p) {s[o] = +1;}
      else        {s[o] = -1;}
    }
  }
}

double Boltzmann(int sm, int ip)
{
  double ene = -J * sm * ( s[Pbc(ip-1)] + s[Pbc(ip+1)] ) - h * sm;
  return ene;
}

void Measure()
{
  int bin;
  double u = 0.0, m = 0.0;

  //looping over spins
  for (int i=0; i<nspin; ++i)
  {
    u += -J * s[i] * s[Pbc(i+1)] - 0.5 * h * (s[i] + s[Pbc(i+1)]);
    m += s[i];
  }
    walker[iu] = u;
    walker[ic] = u * u;
    walker[im] = m;
    walker[ix] = beta * m * m;

    if(varyTemp==0)
    {
      ofstream out;
      out.open("istant_u.dat", ios::app);
      out << u/(double)nspin << endl;
    }
}


void Reset(int iblk) //Reset block averages
{
   
   if(iblk == 1)
   {
      for(int i=0; i<n_props; ++i)
      {
        glob_av[i] = 0;
        glob_av2[i] = 0;
      }
   }

   for(int i=0; i<n_props; ++i)
   {
     blk_av[i] = 0;
   }
   blk_norm = 0;
   attempted = 0;
   accepted = 0;
}


void Accumulate(void) //Update block averages
{
   for(int i=0; i<n_props; ++i)
   {
     blk_av[i] = blk_av[i] + walker[i];
   }
   blk_norm = blk_norm + 1.0;
}


void Averages(int iblk) //Print results for current block
{
    
 ofstream Ene, Heat, Mag, Chi;
 const int wd=12;
  
  // For debugging
  //cout << "Block number " << iblk << endl;
  //cout << "Acceptance rate " << accepted/attempted << endl << endl;
  
  // Use temperature- and algorithm-specific output files
  string prefix = GetOutputPrefix();

  Ene.open((prefix + ".ene.0").c_str(), ios::app);
  stima_u = blk_av[iu]/blk_norm/(double)nspin; //Energy
  glob_av[iu]  += stima_u;
  glob_av2[iu] += stima_u*stima_u;
  err_u=Error(glob_av[iu],glob_av2[iu],iblk);
  Ene << setw(wd) << iblk <<  setw(wd) << stima_u << setw(wd) << glob_av[iu]/(double)iblk << setw(wd) << err_u << endl;
  Ene.close();

  Heat.open((prefix + ".heat.0").c_str(), ios::app);
  stima_c = beta*beta*(blk_av[ic]/blk_norm - pow(blk_av[iu]/blk_norm,2))/(double)nspin; // Heat capacity
  glob_av[ic]  += stima_c;
  glob_av2[ic] += stima_c*stima_c;
  err_c = Error(glob_av[ic],glob_av2[ic],iblk);
  if(varyTemp==1){
    if(iblk==20) Heat << setw(wd) << iblk << setw(wd) << stima_c << setw(wd) << setw(wd) << glob_av[ic]/(double)iblk << setw(wd) << err_c << endl;
  }else{
    Heat << setw(wd) << iblk << setw(wd) << stima_c << setw(wd) << setw(wd) << glob_av[ic]/(double)iblk << setw(wd) << err_c << endl;
  }
  Heat.close();

  Mag.open((prefix + ".mag.0").c_str(), ios::app);
  stima_m = blk_av[im]/blk_norm/(double)nspin; // Magnetization
  glob_av[im]  += stima_m;
  glob_av2[im] += stima_m*stima_m;
  err_m = Error(glob_av[im],glob_av2[im],iblk);
  if(varyTemp==1){
    if(iblk==20) Mag << setw(wd) << iblk << setw(wd) << stima_m << setw(wd) << glob_av[im]/(double)iblk << setw(wd) << err_m << endl;
  }else{
    Mag << setw(wd) << iblk << setw(wd) << stima_m << setw(wd) << glob_av[im]/(double)iblk << setw(wd) << err_m << endl;
  }
  Mag.close();

  Chi.open((prefix + ".chi.0").c_str(), ios::app);
  stima_x = blk_av[ix]/blk_norm/(double)nspin; // Susceptibility
  glob_av[ix]  += stima_x;
  glob_av2[ix] += stima_x*stima_x;
  err_x = Error(glob_av[ix],glob_av2[ix],iblk);
  if(varyTemp==1){
    if(iblk==20) Chi << setw(wd) << iblk << setw(wd) << stima_x << setw(wd) << glob_av[ix]/(double)iblk << setw(wd) << err_x << endl;
  }else{
    Chi << setw(wd) << iblk << setw(wd) << stima_x << setw(wd) << glob_av[ix]/(double)iblk << setw(wd) << err_x << endl;
  }
  Chi.close();

  //cout << "----------------------------" << endl << endl;
}


void ConfFinal(void)
{
  ofstream WriteConf;

  cout << "Print final configuration to file config.final " << endl << endl;
  WriteConf.open("config.final");
  for (int i=0; i<nspin; ++i)
  {
    WriteConf << s[i] << endl;
  }
  WriteConf.close();

  rnd.SaveSeed();
}

int Pbc(int i)  //Algorithm for periodic boundary conditions
{
    if(i >= nspin) i = i - nspin;
    else if(i < 0) i = i + nspin;
    return i;
}

double Error(double sum, double sum2, int iblk)
{
    return sqrt((sum2/(double)iblk - pow(sum/(double)iblk,2))/(double)iblk);
}


void PrintProgress(double progress) 
{
    int barWidth = 50;
    std::cout << "\r[";
    int pos = static_cast<int>(barWidth * progress);
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << std::setw(3) << int(progress * 100.0) << "%";
    std::cout.flush();
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
