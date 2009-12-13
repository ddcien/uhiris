#ifndef MISC_H__
#define MISC_H__

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdexcept>
#include <cstdio>
#include <iostream>

using namespace cv;
using namespace std;
namespace dc{
	const double RADIAN_PER_DEGREE = 0.0174532925;
	const double pi = 3.14159265;
	float bilinear_interpolate(const Mat & src_matrix, const Mat & point);
	void rotate(const Mat& src, Mat& dst, const Point & axis_of_rotation, const Mat& mtx);
	void rotate(const Mat& src, Mat& dst, const Point & axis_of_rotation, float theta);
	Mat contrast_stretch_to_image(const Mat & src);
	Mat fftshift(const Mat & input);
        Mat logPolar(const Mat & image, int size_rho, int size_theta, Point center);
}

#endif
