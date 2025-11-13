#include <cmath>
#include <vector>
#include <iostream>
#include "position.h"
#include "random.h"

using namespace std;

Position::Position(Random* rnd)
{
  x = 0.0;
  y = 0.0;
  z = 0.0;
  random_generator = rnd;
}

Position::Position(double x_val, double y_val, double z_val, Random* rnd)
{
  x = x_val;
  y = y_val;
  z = z_val;
  random_generator = rnd;
}

Position::~Position()
{
  random_generator->SaveSeed();
}

vector<double> Position::get_coordinates()
{
  vector<double> coords(3, 0.0);
  coords.at(0) = x;
  coords.at(1) = y;
  coords.at(2) = z;
  return coords;
}

void Position::set_coordinates(vector<double> coords)
{
  x = coords.at(0);
  y = coords.at(1);
  z = coords.at(2);
}

double Position::get_radius()
{
  return sqrt(x * x + y * y + z * z);
}

double Position::get_theta()
{
  return atan((x * x + y * y) / z);
}

void Position::uniform_step(double stepsize)
{
  double X = random_generator->Rannyu(-stepsize, stepsize);
  double Y = random_generator->Rannyu(-stepsize, stepsize);
  double Z = random_generator->Rannyu(-stepsize, stepsize);

  x += X;
  y += Y;
  z += Z;
}

void Position::gaussian_step(double stepsize)
{
  double X = random_generator->Gauss(0, stepsize);
  double Y = random_generator->Gauss(0, stepsize);
  double Z = random_generator->Gauss(0, stepsize);

  x += X;
  y += Y;
  z += Z;
}

void Position::set_to_zero()
{
  x = 0.0;
  y = 0.0;
  z = 0.0;
}

void Position::print_position()
{
  cout << "Position: (" << x << ", " << y << ", " << z << ")" << endl;
}




