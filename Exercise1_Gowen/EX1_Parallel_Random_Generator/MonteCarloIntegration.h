#ifndef __MonteCarloIntegration__
#define __MonteCarloIntegration__


class MonteCarloIntegration
{

private:

public:

	MonteCarloIntegration();
	~MonteCarloIntegration();

	void firstIntegral(int M, int N, int L, Random rnd, std::vector<double> ave, std::vector<double> ave2, std::vector<double> sumProg, std::vector<double> sumProg2);
	void secondIntegral(int M, int N, int L, Random rnd, std::vector<double> ave, std::vector<double> ave2, std::vector<double> sumProg, std::vector<double> sumProg2);
	void thirdIntegral(int M, int N, int L, Random rnd, std::vector<double> ave, std::vector<double> ave2, std::vector<double> sumProg, std::vector<double> sumProg2);
	float error(std::vector<double> sumProg, std::vector<double> sumProg2, int i);

};


#endif // __MonteCarloIntegration__