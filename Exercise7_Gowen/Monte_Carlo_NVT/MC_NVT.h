/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
//parameters, observables
#include <string>
#include <vector>
#include "random.h"

using namespace std;

int seed[4];
Random rnd;

//parameters, observables
const int m_props=1000;
int n_props, iv, iw, igofr;
double vtail,ptail,bin_size,nbins,sd;
double walker[m_props];

// averages
double blk_av[m_props],blk_norm,accepted,attempted;
double glob_av[m_props],glob_av2[m_props];
double stima_pot,stima_pres,err_pot,err_press,err_gdir;

//configuration
const int m_part=108;
double x[m_part],y[m_part],z[m_part];

// thermodynamical state
int npart, iprint;
double beta,temp,vol,rho,box,rcut;

// simulation
int nstep, nblk;
double delta;
string restart;

const double pi=3.1415927;

//functions
void Input(void);
void Single_run(void);
void set_restart(string, string);
void Equilibrate_system(int);
void Reset(int);
void Accumulate(void);
void Averages(int);
void Move(void);
void ConfFinal(void);
void ConfXYZ(int);
void Measure(bool);
double Boltzmann(double, double, double, int);
double Pbc(double);
double error(double,double,int);

void printProgressBar(double); // Custom Progress bar 


/*
int seed[4];
Random rnd;

const int m_props=1000;
const double K_B = 1.380649e-23;
const double sigma = 0.34e-9;
const double mass = 39.948*1.66054e-27;
const double epsilon_on_K_B = 120;

//parameters, observables
int n_props, iv, iw, ik,it,ie, igofr;
double vtail,ptail,bin_size,nbins,sd;
double walker[m_props];

// averages
double blk_av[m_props],blk_norm,accepted,attempted;
double glob_av[m_props],glob_av2[m_props];
double stima_pot, stima_kin, stima_etot, stima_temp, stima_pres,err_pot,err_press,err_gdir;

//configuration
const int m_part=108;
double x[m_part],y[m_part],z[m_part],xold[m_part],yold[m_part],zold[m_part];
double vx[m_part],vy[m_part],vz[m_part];

// thermodynamical state
int npart;
double beta,energy,temp,vol,rho,box,rcut;
string restart;    // useful for restarting with r(t-dt) and r(t)

// simulation
int nstep, iprint, nblk;
double delta;

const double pi=3.1415927;

//blocking

void blocking_on_MD(int, int, string, string);
double error(double, double, int);

//functions
void Equilibrate_system(int);
double eval_mean_v2();
void set_restart(string, string);
void rescale_velocities(vector<double>);
void Input(void);
void Move(void);
void ConfFinal(void);
void ConfXYZ(int);
void Measure(bool);
double Force(int, int);
double Pbc(double);
void printProgressBar(double); // Custom Progress bar 

double Boltzmann(double, double, double, int);
void Single_run();
void Reset(int);
void Accumulate(void);
void Averages(int);
*/
/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/