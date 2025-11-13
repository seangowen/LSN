#ifndef _position_h_
#define _position_h_

class Random;  // Forward declaration of the Random class

class Position
{
	private:

    double x, y, z;
    Random* random_generator;

 	public:

    Position(Random* rnd);
    Position(double x, double y, double z, Random* rnd);
    ~Position();

    std::vector<double> get_coordinates();
    void set_coordinates(std::vector<double> coord);
    double get_radius();
    double get_theta();
    void uniform_step(double stepsize);
    void gaussian_step(double stepsize);
    void set_to_zero();
    void print_position();


};


#endif
