#include <cmath>
#include <vector>

#include "hydrogenPdf.h"
#include "position.h"

using namespace std;


double HydrogenPdfGroundState::eval(Position *pos)
{
	return (1./M_PI)*exp(-2.*(pos->get_radius()));
}

double HydrogenPdfExcitedState::eval(Position *pos)
{
	vector<double> coord = pos->get_coordinates();
	return (1./(32.*M_PI))*(coord.at(2)*coord.at(2))*exp(-(pos->get_radius()));
}
