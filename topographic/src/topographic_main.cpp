#include <iostream>
#include "cv.h"
#include "highgui.h"

using std::cout;
using std::endl;
using namespace cv;

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

inline Mat ChebyshevZero(int N) {
	int num_of_elem = 2*N+1;
	Mat filter(num_of_elem, 1, CV_64FC1);
	double const_a = 3*N*N+3*N-1;
	double const_b = (2*N-1)*(2*N+1)*(2*N+3);
	for (int i = 0; i < num_of_elem; ++i)
		filter.at<double>(i,0) = 3*(5*i*i-const_a)/const_b;
	return filter;
}

inline Mat ChebyshevFirst(int N) {
	int num_of_elem = 2*N+1;
	Mat filter(num_of_elem, 1, CV_64FC1);
	double const_a = 3*N*N+3*N-1;
	double const_b = 3*pow(static_cast<double>(N),4)+6*pow(static_cast<double>(N),3)-3*N+1;
	double const_c = (N-1)*N*(N+1)*(N+2)*(2*N-1)*(2*N+1)*(2*N+3);
	for (int i = 0; i < num_of_elem; ++i)
		filter.at<double>(i,0) = 5*(7*const_a*pow(static_cast<double>(i),3)-5*const_b*i)/const_c;
	return filter;
}

inline Mat ChebyshevSecond(int N) {
	int num_of_elem = 2*N+1;
	Mat filter(num_of_elem, 1, CV_64FC1);
	double const_a = N*(N+1);
	double const_b = N*(N+1)*(2*N-1)*(2*N+1)*(2*N+3);
	for (int i = 0; i < num_of_elem; ++i)
		filter.at<double>(i,0) = 30*(3*i*i-const_a)/const_b;
	return filter;
}

int main(int argc, char** argv) {

	int neighborhood_size = 5;
	assert(neighborhood_size % 2 == 1);
	int N = (neighborhood_size - 1) / 2;

	Mat h0 = ChebyshevZero(N);
	Mat h1 = ChebyshevFirst(N);
	Mat h2 = ChebyshevSecond(N);
	Mat h20 = h2 * h0.t();
	Mat h11 = h1 * h1.t();
	Mat h02 = h0 * h2.t();

	VideoCapture cap(0);
	if (!cap.isOpened()) return -1;

	namedWindow("Input RGB Image", CV_WINDOW_AUTOSIZE);
	namedWindow("Gray Image", CV_WINDOW_AUTOSIZE);
	namedWindow("Gaussian Smoothed", CV_WINDOW_AUTOSIZE);
	namedWindow("Derivative", CV_WINDOW_AUTOSIZE);

	for (;;) {

		Mat rgb, gray, img;
		cap >> rgb;
		cvtColor(rgb, gray, CV_RGB2GRAY);
		GaussianBlur(gray, img, Size(15,15), 2.5);

		Mat f20xy, f11xy, f02xy;
		filter2D(img, f20xy, -1, h20);
		filter2D(img, f11xy, -1, h11);
		filter2D(img, f02xy, -1, h02);
		//CvMat *hessian = cvCreateMat(2, 2, CV_64FC1);
		
		imshow("Input RGB Image", rgb);
		imshow("Gray Image", gray);
		imshow("Gaussian Smoothed", img);
		imshow("Derivative", f20xy);
		if (waitKey(30) >= 0) {
			PrintMat(&CvMat(f11xy));
			break;
		}
	}
	
	PrintMat(&CvMat(h20));
	PrintMat(&CvMat(h11));
	PrintMat(&CvMat(h02));
	
	return 0;
}