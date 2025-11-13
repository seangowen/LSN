// ========= EX.2.2 SEAN GOWEN ==========
// --------- Random Walk Functions Collection ----------

#ifndef __RandomWalk__
#define __RandomWalk__

using namespace std;

void random_walk_step(vector<int>&, Random&);
void continuous_random_walk_step(vector<double>&, Random&);

void random_walk(vector<int>&, Random&, int);
void continuous_random_walk(vector<double>&, Random&, int);

double walked_distance(vector<int>);
double continuous_walked_distance(vector<double>);

#endif