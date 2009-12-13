#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <math.h>
#include <vector>
#include <iostream>
#include <string>

#include "Misc.h"
using namespace cv;

int main(int argc, char ** argv){
        if (argc <3){
                printf("usage: %s input_image.png theta\n", argv[0]);
		exit(1);
	}

	// load image
	Mat image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

        Point center = Point(image.cols/2, image.rows/2);
        
	// calculate output size
        int size_theta = atoi(argv[2]);
        int size_rho = image.cols / 2;
        Mat logpolar_image = dc::logPolar(image, size_rho, size_theta, center);

	namedWindow("original", CV_WINDOW_AUTOSIZE);
	imshow("original", image);
	namedWindow("logpolar", CV_WINDOW_AUTOSIZE);
        imshow("logpolar", logpolar_image);
	cvSaveImage("logpolar.png", &IplImage(logpolar_image));
	waitKey(0);

	return 0;
}
