#ifndef DC_HISTOGRAM_H_
#define DC_HISTOGRAM_H_

#include <opencv/cv.h>
#include <stdexcept>
#include <iostream>
#include <stdio.h>

#include "Misc.h"
using namespace cv;

namespace dc{
	class Histogram{
	public:
		Histogram();
		Histogram(Mat histogram_array);
		~Histogram();

		static Histogram fromImage(const Mat & image);
		static Histogram createTriangleHistogram();
		static Histogram createGaussianHistogram(float mean = 128, float sigma = 32);

		const Mat getArray() const;
		Mat shapeTo(const Histogram & another_histogram, const Mat & image);
		
		static Mat computeCDFFromPDF(const Mat & histogram_array);
	private:
		Mat histogram_array_;
	};
}
#endif
