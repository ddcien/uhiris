#include <iostream>
#include <cstdio>
#include <fstream>
#include "cv.h"
#include "highgui.h"
#include "differential.h"

using std::cout;
using std::endl;
using std::ofstream;
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

void DumpMatDouble(const char* filename, const Mat& mat) {
	ofstream fout;
	fout.open(filename, ofstream::out);
	if (fout.fail()) {
		cout << "Could not open " << filename << " for writing." << endl;
		return;
	}
	
	for (int i = 0; i < mat.rows; ++i) {
		for (int j = 0; j < mat.cols; ++j)
			fout << mat.at<double>(i,j) << " ";
		fout << endl;
	}

	fout.close();
}

int main(int argc, char** argv) {

	int neighborhood_size = 5;
	assert(neighborhood_size % 2 == 1);
	int N = (neighborhood_size - 1) / 2;

	
	//float Size5Order1X1Y0[25] = {0.017143,0.0085714,-0,-0.0085714,-0.017143,
	//							 -0.068571,-0.034286,0,0.034286,0.068571,
	//							 -0.097143,-0.048571,0,0.048571,0.097143,
	//							 -0.068571,-0.034286,0,0.034286,0.068571,
	//							 0.017143,0.0085714,-0,-0.0085714,-0.017143};
	//float Size5Order2X2Y0[25] = {-0.02449,0.012245,0.02449,0.012245,-0.02449,
	//							 0.097959,-0.04898,-0.097959,-0.04898,0.097959,
	//							 0.13878,-0.069388,-0.13878,-0.069388,0.13878,
	//							 0.097959,-0.04898,-0.097959,-0.04898,0.097959,
	//							 -0.02449,0.012245,0.02449,0.012245,-0.02449};
	//float Size5Order2X1Y1[25] = {0.04,0.02,-0,-0.02,-0.04,
	//							 0.02,0.01,-0,-0.01,-0.02,
	//							 -0,-0,0,0,0,
	//							 -0.02,-0.01,0,0.01,0.02,
	//							 -0.04,-0.02,0,0.02,0.04};
	//float Size5Order2X0Y2[25] = {-0.02449,0.097959,0.13878,0.097959,-0.02449,
	//							 0.012245,-0.04898,-0.069388,-0.04898,0.012245,
	//							 0.02449,-0.097959,-0.13878,-0.097959,0.02449,
	//							 0.012245,-0.04898,-0.069388,-0.04898,0.012245,
	//							 -0.02449,0.097959,0.13878,0.097959,-0.02449};
	//float Size5Order1X0Y1[25] = {0.017143,-0.068571,-0.097143,-0.068571,0.017143,
	//							 0.0085714,-0.034286,-0.048571,-0.034286,0.0085714,
	//							 -0,0,0,0,-0,
	//							 -0.0085714,0.034286,0.048571,0.034286,-0.0085714,
	//							 -0.017143,0.068571,0.097143,0.068571,-0.017143};
	//Mat h10(5,5,CV_32FC1,Size5Order1X1Y0);
	//Mat h11(5,5,CV_32FC1,Size5Order2X1Y1);
	//Mat h20(5,5,CV_32FC1,Size5Order2X2Y0);
	//Mat h01(5,5,CV_32FC1,Size5Order1X0Y1);
	//Mat h02(5,5,CV_32FC1,Size5Order2X0Y2);

	
	//float fSavGolSize5Order1X1Y0[25] = {-0.04000000f,-0.02000000f,0.00000000f,0.02000000f,0.04000000f,-0.04000000f,-0.02000000f,0.00000000f,0.02000000f,0.04000000f,-0.04000000f,-0.02000000f,0.00000000f,0.02000000f,0.04000000f,-0.04000000f,-0.02000000f,0.00000000f,0.02000000f,0.04000000f,-0.04000000f,-0.02000000f,0.00000000f,0.02000000f,0.04000000f};
	//float fSavGolSize5Order1X0Y1[25] = {-0.04000000f,-0.04000000f,-0.04000000f,-0.04000000f,-0.04000000f,-0.02000000f,-0.02000000f,-0.02000000f,-0.02000000f,-0.02000000f,0.00000000f,0.00000000f,0.00000000f,0.00000000f,0.00000000f,0.02000000f,0.02000000f,0.02000000f,0.02000000f,0.02000000f,0.04000000f,0.04000000f,0.04000000f,0.04000000f,0.04000000f};
	//float fSavGolSize5Order2X2Y0[25] = {0.02857143f,-0.01428571f,-0.02857143f,-0.01428571f,0.02857143f,0.02857143f,-0.01428571f,-0.02857143f,-0.01428571f,0.02857143f,0.02857143f,-0.01428571f,-0.02857143f,-0.01428571f,0.02857143f,0.02857143f,-0.01428571f,-0.02857143f,-0.01428571f,0.02857143f,0.02857143f,-0.01428571f,-0.02857143f,-0.01428571f,0.02857143f};
	//float fSavGolSize5Order2X1Y1[25] = {0.04000000f,0.02000000f,0.00000000f,-0.02000000f,-0.04000000f,0.02000000f,0.01000000f,0.00000000f,-0.01000000f,-0.02000000f,0.00000000f,0.00000000f,0.00000000f,0.00000000f,0.00000000f,-0.02000000f,-0.01000000f,0.00000000f,0.01000000f,0.02000000f,-0.04000000f,-0.02000000f,0.00000000f,0.02000000f,0.04000000f};
	//float fSavGolSize5Order2X0Y2[25] = {0.02857143f,0.02857143f,0.02857143f,0.02857143f,0.02857143f,-0.01428571f,-0.01428571f,-0.01428571f,-0.01428571f,-0.01428571f,-0.02857143f,-0.02857143f,-0.02857143f,-0.02857143f,-0.02857143f,-0.01428571f,-0.01428571f,-0.01428571f,-0.01428571f,-0.01428571f,0.02857143f,0.02857143f,0.02857143f,0.02857143f,0.0285};
	//Mat h10(5,5,CV_32FC1,fSavGolSize5Order1X1Y0);
	//Mat h11(5,5,CV_32FC1,fSavGolSize5Order2X1Y1);
	//Mat h20(5,5,CV_32FC1,fSavGolSize5Order2X2Y0);
	//Mat h01(5,5,CV_32FC1,fSavGolSize5Order1X0Y1);
	//Mat h02(5,5,CV_32FC1,fSavGolSize5Order2X0Y2);

	/** Generate filters */
	Differential filters(Differential::CHEBYSHEV);
	filters.ComputeFilterSet();
	Mat h20 = filters.get_h20();
	Mat h10 = filters.get_h10();
	Mat h01 = filters.get_h01();
	Mat h11 = filters.get_h11();
	Mat h02 = filters.get_h02();

	//PrintMat(&CvMat(h20));
	//PrintMat(&CvMat(h10));
	//PrintMat(&CvMat(h01));
	//PrintMat(&CvMat(h11));
	//PrintMat(&CvMat(h02));

		
	VideoCapture cap(0);
	if (!cap.isOpened()) return -1;
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	//VideoWriter video_writer("out_frame.avi", -1, 5, Size(320, 240), true);
	//if (!video_writer.isOpened()) return -1;
	
	namedWindow("Input RGB Image", CV_WINDOW_AUTOSIZE);
	//namedWindow("Gaussian Smoothed", CV_WINDOW_AUTOSIZE);
	//namedWindow("Magnitude", CV_WINDOW_AUTOSIZE);
	//namedWindow("EV1", CV_WINDOW_AUTOSIZE);
	//namedWindow("EV2", CV_WINDOW_AUTOSIZE);
	
	for (;;) {

		Mat rgb, gray, img, tmp, gray_64fc1;
		//rgb = imread(argv[1]);
		cap >> rgb;
		cvtColor(rgb, gray, CV_RGB2GRAY);
		gray.convertTo(gray_64fc1, CV_64FC1);
		GaussianBlur(gray_64fc1, tmp, Size(15, 15), 2.5);
		GaussianBlur(tmp, img, Size(15, 15), 2.5);
		
		Mat f20xy, f11xy, f02xy, f10x, f01y;
		filter2D(img, f20xy, -1, h20);
		filter2D(img, f10x, -1, h10);
		filter2D(img, f01y, -1, h01);
		filter2D(img, f11xy, -1, h11);
		filter2D(img, f02xy, -1, h02);
		
		Mat label_map(img.rows, img.cols, CV_8UC1, Scalar(0));
		Mat mag(img.rows, img.cols, CV_64FC1);
		Mat ev1(img.rows, img.cols, CV_64FC1);
		Mat ev2(img.rows, img.cols, CV_64FC1);
		for (int i = 0; i < img.rows; ++i)
			for (int j = 0; j < img.cols; ++j) {
				Mat hessian(2, 2, CV_64FC1), eigenvalues;
				hessian.at<double>(0,0) = f20xy.at<double>(i,j);
				hessian.at<double>(0,1) = f11xy.at<double>(i,j);
				hessian.at<double>(1,0) = f11xy.at<double>(i,j);
				hessian.at<double>(1,1) = f02xy.at<double>(i,j);
				eigen(hessian, eigenvalues);
				mag.at<double>(i,j) = sqrt(pow(f10x.at<double>(i,j), 2)+pow(f01y.at<double>(i,j), 2));
				ev1.at<double>(i,j) = eigenvalues.at<double>(0,0);
				ev2.at<double>(i,j) = eigenvalues.at<double>(1,0);
				if (mag.at<double>(i,j) < 1.4 && eigenvalues.at<double>(0,0) > 1 && eigenvalues.at<double>(1,0) > 1) {
					circle(rgb, Point(j,i), 5, CV_RGB(0,255,0));
					//cout << Point(i,j).x << " " << Point(i,j).y << endl;
				}
			}
		
		imshow("Input RGB Image", rgb);
		//video_writer << rgb;
		//imshow("Gaussian Smoothed", img);
		//imshow("Magnitude", mag);
		//imshow("EV1", ev1);
		//imshow("EV2", ev2);
		//waitKey();

		if (waitKey(30) >= 0) break;
	}
	
	return 0;
}
