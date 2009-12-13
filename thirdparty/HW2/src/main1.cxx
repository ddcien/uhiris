#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <cstdio>
#include <string>

#include "HistogramImage.h"
using namespace cv;

int main(int argc, char** argv){
	float std_deviation = 32;
	bool is_target_gaussian = false;

	if (argc < 3){
		printf("usage: %s input_image.tif triangle|gaussian [gaussian_std]\n", argv[0]);
		exit(1);
	} else {
		if(string(argv[2]) == "gaussian"){
			if (argc < 4){
				printf("You choose Gaussian distribution, but you didn't specify the the standard deviation (e.g. 48)");
				exit(1);
			} else {
				printf("Target histogram: Gaussian with standard deviation = %s", argv[3]);
				sscanf(argv[3],"%f", &std_deviation);
				is_target_gaussian = true;
			}
		} else if (string(argv[2]) == "triangle"){
			printf("Target histogram: Triangle");
		} else {
			printf("You can only choose between triangle and gaussian as target histogram.");
			exit(1);
		}
	}

	const string input_image_window = "Input Image";
	const string input_image_histogram_window = "Histogram of Input Image";
	const string target_histogram_window = "Target Histogram";
	const string histogram_shaped_image_window = "New Image";
	const string histogram_shaped_window = "New Image histogram";

	namedWindow(input_image_window, CV_WINDOW_AUTOSIZE);
	namedWindow(input_image_histogram_window, CV_WINDOW_AUTOSIZE);
	namedWindow(target_histogram_window, CV_WINDOW_AUTOSIZE);
	namedWindow(histogram_shaped_image_window, CV_WINDOW_AUTOSIZE);
	namedWindow(histogram_shaped_window, CV_WINDOW_AUTOSIZE);

	Mat input_image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	dc::Histogram input_image_histogram = dc::Histogram::fromImage(input_image);
	dc::HistogramImage histogram_image = dc::HistogramImage(input_image_histogram);

	// Original image and its histogram
    imshow(input_image_histogram_window, histogram_image.getImage());
	imshow(input_image_window, input_image);

	dc::Histogram target_histogram;
	if (is_target_gaussian){
		target_histogram = dc::Histogram::createGaussianHistogram(128,std_deviation);
	} else {
		target_histogram = dc::Histogram::createTriangleHistogram();
	}
	target_histogram.getArray();
	imshow(target_histogram_window, dc::HistogramImage(target_histogram).getImage());

	// Shaping the image and showing the new histogram
	Mat new_image = input_image_histogram.shapeTo(target_histogram, input_image);
	imshow(histogram_shaped_image_window, new_image);
	imshow(histogram_shaped_window, dc::HistogramImage(dc::Histogram::fromImage(new_image)).getImage());

    //cvSaveImage("input_histogram.png",&IplImage(image));
    waitKey(0);
    return 0;
}
