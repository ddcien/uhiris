#ifndef DC_HISTOGRAM_IMAGE_H_
#define DC_HISTOGRAM_IMAGE_H_

#include <string>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdexcept>
#include <iostream>

#include "Histogram.h"

#ifdef WIN32
#define int32_t int
#endif

using namespace std;
using namespace cv;

namespace dc{

    class HistogramImage{
    public:
        HistogramImage(const Histogram & histogram);
        HistogramImage(const string & filename);
        const Mat getImage() const;
        ~HistogramImage();
    private:
		void drawHistogramImage(const Histogram & histogram);

        Mat histogram_image_; //!< A histogram image of the histogram array
    };
}

#endif
