#pragma once

class CGamma
{
public:
	double betacf(double a, double b, double x);
	double betai(double a, double b, double x);
	//double Beta(double z, double a, double b);
	double Gamma( double a, double chi2 );
	CGamma();
	virtual ~CGamma();

	double gcf( double a, double x );
	double gamser( double a, double x );
	double gammln( double xx );
	double lnf( double xx );
};
