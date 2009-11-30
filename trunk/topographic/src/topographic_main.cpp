#include <iostream>
#include "cv.h"

using std::cout;
using std::endl;

inline double ChebyshevZero(double x) {
	return 1.0;
}

inline double ChebyshevFirst(double x) {
	return 2 * x;
}

inline double ChebyshevSecond(double x, int N) {
	return 6 * pow(x, 2.0) + 2 * N * (N + 1);
}

inline double ChebyshevThird(double x, int N) {
	return 20 * pow(x, 3.0) - 4 * (3 * pow(N, 2.0) + 3 * N - 1) * x;
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

	int neighborhood_size = 9;
	assert(neighborhood_size % 2 == 1);
	int N = (neighborhood_size - 1) / 2;

	return 0;
}