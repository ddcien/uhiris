#include <iostream>
#include "cv.h"

using std::cout;
using std::endl;

inline double *ChebyshevZero(int N) {
	int num_of_elem = 2*N+1;
	double *kernel = new double [num_of_elem];
	double const_a = 3*N*N+3*N-1;
	double const_b = (2*N-1)*(2*N+1)*(2*N+3);
	for (int i = 0; i < num_of_elem; ++i)
		kernel[i] = 3*(5*i*i-const_a)/const_b;
	return kernel;
}

inline double *ChebyshevFirst(int N) {
	int num_of_elem = 2*N+1;
	double *kernel = new double [num_of_elem];
	double const_a = 3*N*N+3*N-1;
	double const_b = 3*pow(static_cast<double>(N),4)+6*pow(static_cast<double>(N),3)-3*N+1;
	double const_c = (N-1)*N*(N+1)*(N+2)*(2*N-1)*(2*N+1)*(2*N+3);
	for (int i = 0; i < num_of_elem; ++i)
		kernel[i] = 5*(7*const_a*pow(static_cast<double>(i),3)-5*const_b*i)/const_c;
	return kernel;
}

inline double *ChebyshevSecond(int N) {
	int num_of_elem = 2*N+1;
	double *kernel = new double [num_of_elem];
	double const_a = N*(N+1);
	double const_b = N*(N+1)*(2*N-1)*(2*N+1)*(2*N+3);
	for (int i = 0; i < num_of_elem; ++i)
		kernel[i] = 30*(3*i*i-const_a)/const_b;
	return kernel;
}

int main(int argc, char** argv) {

	//double tmp1[] = {1,1,1,
	//				 4,2,1,
	//				 9,3,1,
	//				 16,4,1,
	//				 25,5,1,
	//				 36,6,1,
	//				 49,7,1,
	//				 64,8,1,
	//				 81,9,1,
	//				 100,10,1};
	//double tmp2[] = {3,11,25,45,71,103,141,185,235,291};
	//CvMat A = cvMat(10, 3, CV_64FC1, tmp1);
	//CvMat b = cvMat(10, 1, CV_64FC1, tmp2);
	//CvMat* x = cvCreateMat(3, 1, CV_64FC1);
	//cvZero(x);
	//int valid = cvSolve(&A, &b, x);
	//
	//for (int i = 0; i < 3; ++i)
	//	cout << cvmGet(x, i, 0) << " ";
	//cout << endl;

	int neighborhood_size = 5;
	assert(neighborhood_size % 2 == 1);
	int N = (neighborhood_size - 1) / 2;

	return 0;
}