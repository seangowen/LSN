#include "random.h"

void Initialize();
void Reset(int);
void Move();
void PrintPos(int);
void Measure();
void Accumulate();
void Average(int);
void Report(int);
void ReadInput();
void Welcome();

double wavefunction(double, double, double);
double pdf(double, double, double);
double eloc(double, double, double);
double Error(double, double, int);
double x0, x, x_new;
int n_blks, n_steps, iprint, imeasure;
double delta;

double mu, sigma;

const int m_props=1;
int n_props, iu;
double walker[m_props];
double blk_av[m_props],blk_norm,accepted,attempted;
double glob_av[m_props],glob_av2[m_props];
double stima_u;
double err_u;

// RNG
Random rnd;
int seed[4];
int p1, p2;
