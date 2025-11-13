#ifndef __Buffon__
#define __Buffon__


class Buffon
{
private:

public:

	Buffon();
	~Buffon();

	void blocked_statistics(std::vector<double>  AV, std::vector<double>  AV2, int N, std::string filename);
	bool crosses_spacing(double y1, double y2, double spacing);
	float error(std::vector<double> AV, std::vector<double> AV2, int n);
	float pi_computation(int hits, int n_throws, float d, float length);
};

#endif // __Buffon__