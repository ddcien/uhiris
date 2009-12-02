#include <iostream>
#include "cv.h"

using std::cout;
using std::endl;

void PrintMat(CvMat *A)
/** http://blog.weisu.org/2007/11/opencv-print-matrix.html */
{
	int i, j;
	for (i = 0; i < A->rows; i++)
	{
		printf("\n");
		switch (CV_MAT_DEPTH(A->type))
		{
		case CV_32F:
		case CV_64F:
			for (j = 0; j < A->cols; j++)
				printf ("%8.3f ", (float)cvGetReal2D(A, i, j));
			break;
		case CV_8U:
		case CV_16U:
			for(j = 0; j < A->cols; j++)
				printf ("%6d",(int)cvGetReal2D(A, i, j));
			break;
		default:
			break;
		}
	}
	printf("\n");
}

inline CvMat *ChebyshevZero(int N) {
	int num_of_elem = 2*N+1;
	CvMat *filter = cvCreateMat(num_of_elem, 1, CV_64FC1);
	double const_a = 3*N*N+3*N-1;
	double const_b = (2*N-1)*(2*N+1)*(2*N+3);
	for (int i = 0; i < num_of_elem; ++i)
		cvmSet(filter, i, 0, 3*(5*i*i-const_a)/const_b);
	return filter;
}

inline CvMat *ChebyshevFirst(int N) {
	int num_of_elem = 2*N+1;
	CvMat *filter = cvCreateMat(num_of_elem, 1, CV_64FC1);
	double const_a = 3*N*N+3*N-1;
	double const_b = 3*pow(static_cast<double>(N),4)+6*pow(static_cast<double>(N),3)-3*N+1;
	double const_c = (N-1)*N*(N+1)*(N+2)*(2*N-1)*(2*N+1)*(2*N+3);
	for (int i = 0; i < num_of_elem; ++i)
		cvmSet(filter, i, 0, 5*(7*const_a*pow(static_cast<double>(i),3)-5*const_b*i)/const_c);
	return filter;
}

inline CvMat *ChebyshevSecond(int N) {
	int num_of_elem = 2*N+1;
	CvMat *filter = cvCreateMat(num_of_elem, 1, CV_64FC1);
	double const_a = N*(N+1);
	double const_b = N*(N+1)*(2*N-1)*(2*N+1)*(2*N+3);
	for (int i = 0; i < num_of_elem; ++i)
		cvmSet(filter, i, 0, 30*(3*i*i-const_a)/const_b);
	return filter;
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

	CvMat *h0 = ChebyshevZero(N);
	CvMat *h1 = ChebyshevFirst(N);
	CvMat *h2 = ChebyshevSecond(N);
	CvMat *h20 = cvCreateMat(neighborhood_size, neighborhood_size, CV_64FC1);
	CvMat *h02 = cvCreateMat(neighborhood_size, neighborhood_size, CV_64FC1);;
	CvMat *h11 = cvCreateMat(neighborhood_size, neighborhood_size, CV_64FC1);;
	CvMat *tmp = cvCreateMat(1, neighborhood_size, CV_64FC1);
	cvTranspose(h0, tmp);
	cvMatMul(h2, tmp, h20);
	cvTranspose(h1, tmp);
	cvMatMul(h1, tmp, h11);
	cvTranspose(h2, tmp);
	cvMatMul(h0, tmp, h02);
	CvMat *hessian = cvCreateMat(2, 2, CV_64FC1);

	
	PrintMat(h20);
	PrintMat(h11);
	PrintMat(h02);
	return 0;
}