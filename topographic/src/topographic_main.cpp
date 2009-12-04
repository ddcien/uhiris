#include <iostream>
#include <cstdio>
#include "cv.h"
#include "highgui.h"

using std::cout;
using std::endl;
using namespace cv;

enum Topographic {PEAK, PIT, RIDGESADDLE, RAVINESADDLE, RIDGE, RAVINE, FLAT, CONVEXHILL, CONCAVEHILL, CONCAVESADDLEHILL, CONVEXSADDLEHILL, SLOPEHILL};

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
	Mat filter(2*N+1, 1, CV_64FC1);
	double const_a = 3*N*N+3*N-1;
	double const_b = (2*N-1)*(2*N+1)*(2*N+3);
	for (int i = -N; i <= N; ++i)
		filter.at<double>(i+N,0) = 3*(5*i*i-const_a)/const_b;
	return filter;
}

inline Mat ChebyshevFirst(int N) {
	Mat filter(2*N+1, 1, CV_64FC1);
	double const_a = 3*N*N+3*N-1;
	double const_b = 3*pow(static_cast<double>(N),4)+6*pow(static_cast<double>(N),3)-3*N+1;
	double const_c = (N-1)*N*(N+1)*(N+2)*(2*N-1)*(2*N+1)*(2*N+3);
	for (int i = -N; i <= N; ++i)
		filter.at<double>(i+N,0) = 5*(7*const_a*pow(static_cast<double>(i),3)-5*const_b*i)/const_c;
	return filter;
}

inline Mat ChebyshevSecond(int N) {
	Mat filter(2*N+1, 1, CV_64FC1);
	double const_a = N*(N+1);
	double const_b = N*(N+1)*(2*N-1)*(2*N+1)*(2*N+3);
	for (int i = -N; i <= N; ++i)
		filter.at<double>(i+N,0) = 30*(3*i*i-const_a)/const_b;
	return filter;
}

int main(int argc, char** argv) {

	int neighborhood_size = 21;
	assert(neighborhood_size % 2 == 1);
	int N = (neighborhood_size - 1) / 2;

	Mat h0 = ChebyshevZero(N);
	Mat h1 = ChebyshevFirst(N);
	Mat h2 = ChebyshevSecond(N);
	Mat h20 = h2 * h0.t();
	Mat h10 = h1 * h0.t();
	Mat h01 = h0 * h1.t();
	Mat h11 = h1 * h1.t();
	Mat h02 = h0 * h2.t();

	cvSave("h20.txt",&CvMat(h20));
	PrintMat(&CvMat(h0));
	PrintMat(&CvMat(h1));
	PrintMat(&CvMat(h2));
	PrintMat(&CvMat(h20));
	PrintMat(&CvMat(h02));

	//VideoCapture cap(0);
	//if (!cap.isOpened()) return -1;
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

	namedWindow("Input RGB Image", CV_WINDOW_AUTOSIZE);
	//namedWindow("Gray Image", CV_WINDOW_AUTOSIZE);
	namedWindow("Gaussian Smoothed", CV_WINDOW_AUTOSIZE);
	namedWindow("Magnitude", CV_WINDOW_AUTOSIZE);
	namedWindow("EV1", CV_WINDOW_AUTOSIZE);
	namedWindow("EV2", CV_WINDOW_AUTOSIZE);
	
//	for (;;) {

		Mat rgb, gray, img, tmp;
		rgb = imread(argv[1]);
//		cap >> rgb;
		cvtColor(rgb, gray, CV_RGB2GRAY);
		GaussianBlur(gray, tmp, Size(25, 25), 20);
		GaussianBlur(tmp, img, Size(25, 25), 20);

		Mat img_64fc1, f20xy, f11xy, f02xy, f10x, f01y;
		img.convertTo(img_64fc1, CV_64FC1);
		filter2D(img_64fc1, f20xy, -1, h20);
		filter2D(img_64fc1, f10x, -1, h10);
		filter2D(img_64fc1, f01y, -1, h01);
		filter2D(img_64fc1, f11xy, -1, h11);
		filter2D(img_64fc1, f02xy, -1, h02);
		
		Mat label_map(img.rows, img.cols, CV_8UC1, Scalar(0));
		Mat mag(img.rows, img.cols, CV_64FC1);
		Mat ev1(img.rows, img.cols, CV_64FC1);
		Mat ev2(img.rows, img.cols, CV_64FC1);
		for (int i = 0; i < img.rows; ++i)
			for (int j = 0; j < img.cols; ++j) {
				Mat hessian = cvCreateMat(2, 2, CV_64FC1), eigenvalues;
				hessian.at<double>(0,0) = f20xy.at<double>(i,j);
				hessian.at<double>(0,1) = f11xy.at<double>(i,j);
				hessian.at<double>(1,0) = f11xy.at<double>(i,j);
				hessian.at<double>(1,1) = f02xy.at<double>(i,j);
				eigen(hessian, eigenvalues);
				mag.at<double>(i,j) = sqrt(pow(f10x.at<double>(i,j), 2)+pow(f01y.at<double>(i,j), 2));
				ev1.at<double>(i,j) = eigenvalues.at<double>(0,0);
				ev2.at<double>(i,j) = eigenvalues.at<double>(1,0);
				if (mag.at<double>(i,j) < 0.1 && eigenvalues.at<double>(0,0) > 0.1 && eigenvalues.at<double>(1,0) > 0.1) {
					circle(rgb, Point(j,i), 5, CV_RGB(0,255,0));
					//cout << Point(i,j).x << " " << Point(i,j).y << endl;
				}
			}
		
		imshow("Input RGB Image", rgb);
		//imshow("Gray Image", gray);
		imshow("Gaussian Smoothed", img);
		imshow("Magnitude", mag);
		imshow("EV1", ev1);
		imshow("EV2", ev2);
		waitKey();
	//	if (waitKey(30) >= 0) break;
	//}

	
	//PrintMat(&CvMat(h20));
	//PrintMat(&CvMat(h11));
	//PrintMat(&CvMat(h02));
	
	return 0;
}
