#include "differential.h"
#include <iostream>

using std::cout;
using std::endl;

/** Filters from MATLAB
	http://www.mathworks.com/matlabcentral/fileexchange/9123-2-d-savitzky-golay-smoothing-and-differentiation-filter
*/
const float Differential::mwSavGolSize5Order1X1Y0_[25] = {0.017143f,0.0085714f,-0.0f,-0.0085714f,-0.017143f,
														  -0.068571f,-0.034286f,0.0f,0.034286f,0.068571f,
														  -0.097143f,-0.048571f,0.0f,0.048571f,0.097143f,
														  -0.068571f,-0.034286f,0.0f,0.034286f,0.068571f,
														  0.017143f,0.0085714f,-0.0f,-0.0085714f,-0.017143f};
const float Differential::mwSavGolSize5Order2X2Y0_[25] = {-0.02449f,0.012245f,0.02449f,0.012245f,-0.02449f,
														  0.097959f,-0.04898f,-0.097959f,-0.04898f,0.097959f,
														  0.13878f,-0.069388f,-0.13878f,-0.069388f,0.13878f,
														  0.097959f,-0.04898f,-0.097959f,-0.04898f,0.097959f,
														  -0.02449f,0.012245f,0.02449f,0.012245f,-0.02449f};
const float Differential::mwSavGolSize5Order2X1Y1_[25] = {0.04f,0.02f,-0.0f,-0.02f,-0.04f,
														  0.02f,0.01f,-0.0f,-0.01f,-0.02f,
														  -0.0f,-0.0f,0.0f,0.0f,0.0f,
														  -0.02f,-0.01f,0.0f,0.01f,0.02f,
														  -0.04f,-0.02f,0.0f,0.02f,0.04f};
const float Differential::mwSavGolSize5Order2X0Y2_[25] = {-0.02449f,0.097959f,0.13878f,0.097959f,-0.02449f,
														  0.012245f,-0.04898f,-0.069388f,-0.04898f,0.012245f,
														  0.02449f,-0.097959f,-0.13878f,-0.097959f,0.02449f,
														  0.012245f,-0.04898f,-0.069388f,-0.04898f,0.012245f,
														  -0.02449f,0.097959f,0.13878f,0.097959f,-0.02449f};
const float Differential::mwSavGolSize5Order1X0Y1_[25] = {0.017143f,-0.068571f,-0.097143f,-0.068571f,0.017143f,
														  0.0085714f,-0.034286f,-0.048571f,-0.034286f,0.0085714f,
														  -0.0f,0.0f,0.0f,0.0f,-0.0f,
														  -0.0085714f,0.034286f,0.048571f,0.034286f,-0.0085714f,
														  -0.017143f,0.068571f,0.097143f,0.068571f,-0.017143f};

/** Filters from 
	http://research.microsoft.com/en-us/um/people/jckrumm/SavGol/SavGol.htm
*/
const float Differential::msSavGolSize5Order1X1Y0_[25] = {-0.04000000f,-0.02000000f,0.00000000f,0.02000000f,0.04000000f,-0.04000000f,-0.02000000f,0.00000000f,0.02000000f,0.04000000f,-0.04000000f,-0.02000000f,0.00000000f,0.02000000f,0.04000000f,-0.04000000f,-0.02000000f,0.00000000f,0.02000000f,0.04000000f,-0.04000000f,-0.02000000f,0.00000000f,0.02000000f,0.04000000f};
const float Differential::msSavGolSize5Order1X0Y1_[25] = {-0.04000000f,-0.04000000f,-0.04000000f,-0.04000000f,-0.04000000f,-0.02000000f,-0.02000000f,-0.02000000f,-0.02000000f,-0.02000000f,0.00000000f,0.00000000f,0.00000000f,0.00000000f,0.00000000f,0.02000000f,0.02000000f,0.02000000f,0.02000000f,0.02000000f,0.04000000f,0.04000000f,0.04000000f,0.04000000f,0.04000000f};
const float Differential::msSavGolSize5Order2X2Y0_[25] = {0.02857143f,-0.01428571f,-0.02857143f,-0.01428571f,0.02857143f,0.02857143f,-0.01428571f,-0.02857143f,-0.01428571f,0.02857143f,0.02857143f,-0.01428571f,-0.02857143f,-0.01428571f,0.02857143f,0.02857143f,-0.01428571f,-0.02857143f,-0.01428571f,0.02857143f,0.02857143f,-0.01428571f,-0.02857143f,-0.01428571f,0.02857143f};
const float Differential::msSavGolSize5Order2X1Y1_[25] = {0.04000000f,0.02000000f,0.00000000f,-0.02000000f,-0.04000000f,0.02000000f,0.01000000f,0.00000000f,-0.01000000f,-0.02000000f,0.00000000f,0.00000000f,0.00000000f,0.00000000f,0.00000000f,-0.02000000f,-0.01000000f,0.00000000f,0.01000000f,0.02000000f,-0.04000000f,-0.02000000f,0.00000000f,0.02000000f,0.04000000f};
const float Differential::msSavGolSize5Order2X0Y2_[25] = {0.02857143f,0.02857143f,0.02857143f,0.02857143f,0.02857143f,-0.01428571f,-0.01428571f,-0.01428571f,-0.01428571f,-0.01428571f,-0.02857143f,-0.02857143f,-0.02857143f,-0.02857143f,-0.02857143f,-0.01428571f,-0.01428571f,-0.01428571f,-0.01428571f,-0.01428571f,0.02857143f,0.02857143f,0.02857143f,0.02857143f,0.02857143f};

Differential::Differential() 
	: type_(CHEBYSHEV), halfspan_(2) {}

Differential::Differential( FilterType type ) 
	: type_(type), halfspan_(2) {}

Differential::Differential( FilterType type, int halfspan ) 
	: type_(type), halfspan_(halfspan) {}

bool Differential::ComputeFilterSet()
{
	Mat h0, h1, h2;
	switch (type_) {
		case CHEBYSHEV:
			h0 = ChebyshevZero(halfspan_);
			h1 = ChebyshevFirst(halfspan_);
			h2 = ChebyshevSecond(halfspan_);
			h20_ = -h2 * h0.t();
			h10_ = -h1 * h0.t();
			h01_ = -h0 * h1.t();
			h11_ = -h1 * h1.t();
			h02_ = -h0 * h2.t();
			break;
		case SAVGOLMATHWORK:
			h10_ = Mat(5,5,CV_32FC1,const_cast<float *>(mwSavGolSize5Order1X1Y0_));
			h11_ = Mat(5,5,CV_32FC1,const_cast<float *>(mwSavGolSize5Order2X1Y1_));
			h20_ = Mat(5,5,CV_32FC1,const_cast<float *>(mwSavGolSize5Order2X2Y0_));
			h01_ = Mat(5,5,CV_32FC1,const_cast<float *>(mwSavGolSize5Order1X0Y1_));
			h02_ = Mat(5,5,CV_32FC1,const_cast<float *>(mwSavGolSize5Order2X0Y2_));
			break;
		case SAVGOLMICROSOFT:
			h10_ = Mat(5,5,CV_32FC1,const_cast<float *>(msSavGolSize5Order1X1Y0_));
			h11_ = Mat(5,5,CV_32FC1,const_cast<float *>(msSavGolSize5Order2X1Y1_));
			h20_ = Mat(5,5,CV_32FC1,const_cast<float *>(msSavGolSize5Order2X2Y0_));
			h01_ = Mat(5,5,CV_32FC1,const_cast<float *>(msSavGolSize5Order1X0Y1_));
			h02_ = Mat(5,5,CV_32FC1,const_cast<float *>(msSavGolSize5Order2X0Y2_));
			break;
		default:
			cout << "Invalid filter type." << endl;
			return false;
	}
	return true;
}

Differential::~Differential()
{}

inline Mat Differential::ChebyshevZero( int N )
{
	Mat filter(2*N+1, 1, CV_32FC1);
	float const_a = 3*N*N+3*N-1;
	float const_b = (2*N-1)*(2*N+1)*(2*N+3);
	for (int i = -N; i <= N; ++i)
		filter.at<float>(i+N,0) = 3*(5*i*i-const_a)/const_b;
	return filter;
}

inline Mat Differential::ChebyshevFirst( int N )
{
	Mat filter(2*N+1, 1, CV_32FC1);
	float const_a = 3*N*N+3*N-1;
	float const_b = 3*pow(static_cast<float>(N),4)+6*pow(static_cast<float>(N),3)-3*N+1;
	float const_c = (N-1)*N*(N+1)*(N+2)*(2*N-1)*(2*N+1)*(2*N+3);
	for (int i = -N; i <= N; ++i)
		filter.at<float>(i+N,0) = 5*(7*const_a*pow(static_cast<float>(i),3)-5*const_b*i)/const_c;
	return filter;
}

inline Mat Differential::ChebyshevSecond( int N )
{
	Mat filter(2*N+1, 1, CV_32FC1);
	float const_a = N*(N+1);
	float const_b = N*(N+1)*(2*N-1)*(2*N+1)*(2*N+3);
	for (int i = -N; i <= N; ++i)
		filter.at<float>(i+N,0) = 30*(3*i*i-const_a)/const_b;
	return filter;
}