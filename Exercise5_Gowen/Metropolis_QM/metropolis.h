#ifndef _metropolis_h_
#define _metropolis_h_

#include "random.h"
#include "position.h"
#include "hydrogenPdf.h"
#include "measure.h"
#include <string>
#include <vector>

using namespace std;

class Metropolis : public Measure
{
private:
    int steps;               // total number of Metropolis steps
    int accepted;            // number of accepted moves
    int attempted;           // number of attempted moves
    double step_size;        // step size for the move
    string method;           // type of proposal ("uniform" or "gaussian")

    HydrogenPdf* p_function; // pointer to probability density function
    Position* x;             // current position
    Random* random_generator;      // random number generator

public:
    Metropolis(int N, HydrogenPdf* mypdf, Position* start, Random* rnd, double stepsize, string method);
    ~Metropolis();

    void equilibrate(int Nequi);
    void trial_step();
    void run(string filename);
    double acceptance_rate();
    vector<double> get_measure();
    void get_measure_all(int Nequi, std::string filename);

    int get_dimension();
};

#endif