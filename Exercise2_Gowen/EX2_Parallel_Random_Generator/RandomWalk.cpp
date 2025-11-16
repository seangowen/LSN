// ========= EX.2.2 SEAN GOWEN ==========
// --------- Random Walk Functions Collection ---------- 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "random.h"
#include "RandomWalk.h"

using namespace std;


void random_walk_step(vector<int>& position, Random& rnd)
{
	double r;
	r = rnd.Rannyu();

	// we divide the value of r=[0,1.0] in 6 equal parts. 
	// Depending on what range it belongs to we take a step forward/backward in x,y or z
	if (r < 1./6) { position[0] +=1; }
	else if (1./6 <= r && r < 2./6) { position[0] -=1; }
	else if (2./6 <= r && r < 3./6) { position[1] +=1; }
	else if (3./6 <= r && r < 4./6) { position[1] -=1; }
	else if (4./6 <= r && r < 5./6) { position[2] +=1; }
	else { position[2] -=1; }
}


void continuous_random_walk_step(vector<double>& sphere_position, Random& rnd)
{
    double theta, phi;

    // Uniform azimuth
    phi = rnd.Rannyu(0, 2*M_PI);

    // Polar angle sampled for uniform sphere distribution
    theta = acos(1 - 2*rnd.Rannyu());  // theta = arccos(1 - 2*u)

    // Update position
    sphere_position[0] += 10*sin(theta) * cos(phi);
    sphere_position[1] += 10*sin(theta) * sin(phi);
    sphere_position[2] += 10*cos(theta);
}


void random_walk(vector<int>& position, Random& rnd, int steps)
{
	for (int i=0; i<steps; i++)
	{
		random_walk_step(position, rnd);
	}
}


void continuous_random_walk(vector<double> & sphere_position, Random & rnd, int N)
{
	for (int i = 0; i < N; i++)
	{
      continuous_random_walk_step(sphere_position, rnd);
   	}
}


double walked_distance(vector<int> position)
{
	float distance = position[0]*position[0] + position[1]*position[1] + position[2]*position[2];

	return distance;
}

double continuous_walked_distance(vector<double> sphere_position)
{
	float distance = sphere_position[0]*sphere_position[0] + sphere_position[1]*sphere_position[1] + sphere_position[2]*sphere_position[2];

	return distance;

}




