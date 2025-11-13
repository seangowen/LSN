#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include "metropolis.h"
#include "hydrogenPdf.h"
#include "measure.h"

using namespace std;

Metropolis::Metropolis(int N, HydrogenPdf* mypdf, Position* start, Random* rnd, double stepsize, string method)
{
    steps = N;
    accepted = 0;
    attempted = 0;
    step_size = stepsize;
    this->method = method;

    x = start;
    p_function = mypdf;
    random_generator = rnd;
}

Metropolis::~Metropolis()
{
    random_generator->SaveSeed();
}


void Metropolis::equilibrate(int Nequi)
{
    ofstream out("./../Data/equilibration_steps.dat");

    for (int i = 0; i < Nequi; i++)
    {
        trial_step();
        out << x->get_radius() << endl;
    }

    out.close();
}

void Metropolis::trial_step()
{
    vector<double> old = x->get_coordinates();
    double A = p_function->eval(x);

    if (method == "uniform")  x->uniform_step(step_size);
    if (method == "gaussian") x->gaussian_step(step_size);

    double B = p_function->eval(x);
    double alpha = min(1.0, B / A);

    double rand_val = random_generator->Rannyu();

    if (rand_val <= alpha) accepted++;
    else x->set_coordinates(old);

    attempted++;
}

void Metropolis::run(string filename)
{
    ofstream out(filename);

    for (int i = 0; i < steps; i++)
    {
        trial_step();
        vector<double> coord = x->get_coordinates();
        out << coord.at(0) << "   " << coord.at(1) << "   " << coord.at(2) << endl;
    }

    out.close();
}

double Metropolis::acceptance_rate()
{
    return double(accepted) / double(attempted);
}

vector<double> Metropolis::get_measure()
{
    trial_step();
    vector<double> meas;
    meas.push_back(x->get_radius());
    return meas;
}

void Metropolis::get_measure_all(int Nequi, std::string filename)
{
    std::ofstream out(filename);
    if (!out.is_open())
    {
        std::cerr << "Error: cannot open " << filename << std::endl;
        return;
    }

    // Equilibration phase (recording all points)
    for (int i = 0; i < Nequi; i++)
    {
        trial_step();
        std::vector<double> coord = x->get_coordinates();
        out << coord.at(0) << "   " << coord.at(1) << "   " << coord.at(2) << "   "
            << x->get_radius() << std::endl;
    }

    // Production phase (also recorded)
    for (int i = 0; i < steps; i++)
    {
        trial_step();
        std::vector<double> coord = x->get_coordinates();
        out << coord.at(0) << "   " << coord.at(1) << "   " << coord.at(2) << "   "
            << x->get_radius() << std::endl;
    }

    out.close();
}


int Metropolis::get_dimension()
{
    return 1;
}
