#include <iostream>
#include <string>
#include "metropolis.h"
#include "random.h"
#include "blockedStatistics.h"
#include "measure.h"
#include "hydrogenPdf.h"


using namespace std;


int main(int argc, char* argv[])
{

	int M = 1e7; // Total number of steps 

	// Defining different step sizes of the Metropolis Algorithm
	// to get the right acceptance rate 
	double step_100 [] = {1.2, 0.7};
	double step_210 [] = {2.9, 1.9};

	string method [] = {"uniform", "gaussian"};

  	Random* rnd = new Random();
  	rnd->Init();
  	BlockedStatistics *mystat = new BlockedStatistics();


  	Position *start  = new Position(0.,0.,0., rnd);
  	vector<double> coord  = start->get_coordinates();
  	vector<double> origin(3,0.);


	cout << "Running The Metropolis algorithm" << endl;
	cout << "Sampling the Ground State |100> and the 1st Excited State |210> of the Hydrogen. " << endl;

	HydrogenPdf* hydrogen = new HydrogenPdfGroundState();
	Metropolis *Metro_100 = new Metropolis(M, hydrogen, start, rnd, 1.235, "uniform");
	Metro_100->equilibrate(1000);
	Metro_100->run("./../Data/H100.dat");
	cout << "Acceptance rate for |100>: " << Metro_100->acceptance_rate() << endl;

	start->set_coordinates(coord);                              // starting from the old start


	hydrogen = new HydrogenPdfExcitedState();
	Metropolis *Metro_210 = new Metropolis(M, hydrogen, start, rnd, 2.955, "uniform");
	Metro_210->equilibrate(1000);
	Metro_210->run("./../Data/H210.dat");
	cout << "Acceptance rate for |210>: " << Metro_210->acceptance_rate() << endl;



  	cout << "Evaluating expected values for |Psi_100|^2 and |Psi_210|^2 with the blocking method." << endl;
  	cout << endl;

  	// Looping through both transition methods
  	for(int i=0; i<2; i++)
  	{
	    // Starting point (0,0,0)
	    start->set_coordinates(coord);

	    cout << "Running blocking for |100> and |210> using " << method[i] << " steps." << endl;
	    hydrogen = new HydrogenPdfGroundState();
	    Measure *measure_100 = new Metropolis(M, hydrogen, start, rnd, step_100[i], method[i]);
	    string path = "./../Data/ground_state_"+method[i]+"_transition_blocked_stats.dat";

	    mystat->blocking(M, 100, measure_100, path);
	    cout << endl;

	    start->set_coordinates(coord);
	    hydrogen = new HydrogenPdfExcitedState();
	    Measure *measure_210 = new Metropolis(M, hydrogen, start, rnd, step_210[i], method[i]);
	    path = "./../Data/excited_state_"+method[i]+"_transition_blocked_stats.dat";
	    mystat->blocking(M, 100, measure_210, path);
  	}


  	// ---------------------------------------------------------------------
  	// Recording all sampled points, including those during equilibration
  	// ---------------------------------------------------------------------

  	HydrogenPdf* anotherHydrogen = new HydrogenPdfGroundState(); 	// another hydrogen (pointer) for the test
	Position *start_far = new Position(100.0, 100.0, 100.0, rnd);  // far from the origin
	Metropolis *metro_test = new Metropolis(1e5, anotherHydrogen, start_far, rnd, 1.2, "uniform");

	cout << "Recording all steps (including equilibration)..." << endl;
	metro_test->get_measure_all(5000, "./../Data/metropolis_all_far_start.dat");



	// Deleting all the pointers!

	// Delete Metropolis objects
	delete Metro_100;
	delete Metro_210;
	delete metro_test;

	// Delete HydrogenPdf objects
	delete hydrogen;
	delete anotherHydrogen;

	// Delete Position objects
	delete start;
	delete start_far;

	// Delete Random generator
	delete rnd;

	return 0;
}