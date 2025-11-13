#ifndef _hydrogenPdf_h_
#define _hydrogenPdf_h_

class Position; // Forward declaration of the Random class

// Virtual Class (a.k.a Base class)
class HydrogenPdf
{
	public:
    	virtual double eval(Position *pos)=0;
    	virtual ~HydrogenPdf() {}
};


class HydrogenPdfGroundState : public HydrogenPdf
{
 	public:
		double eval(Position *pos);
};


class HydrogenPdfExcitedState : public HydrogenPdf
{
 	public:
    	double eval(Position *pos);
};

#endif