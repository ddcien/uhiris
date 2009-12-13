#include "HistogramImage.h"

/** \brief Construct a HistogramImage given a histogram
  \param histogram_array a Nx1 1-channel histogram
*/
dc::HistogramImage::HistogramImage(const Histogram & histogram)
{
	//sanity check
	Mat histogram_array = histogram.getArray();
	if (histogram_array.rows == 1 || histogram_array.cols == 1){
		drawHistogramImage(histogram);	
	} else {
		throw std::invalid_argument("You should pass in a Nx1 histogram array");
	}
}

/** \brief Construct a HistogramImage from an image file
  This method create a histogram array internally
*/
dc::HistogramImage::HistogramImage(const string & filename){
    Mat image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
    Histogram histogram = dc::Histogram::fromImage(image);
    drawHistogramImage(histogram);
}

dc::HistogramImage::~HistogramImage()
{
    // With the new OpenCV 2.0, memory deallocation is automatic
}

const Mat dc::HistogramImage::getImage() const
{
    return histogram_image_;
}

/** \brief Compute Histogram image from input histogram array
*/
void dc::HistogramImage::drawHistogramImage( const Histogram & histogram )
{
	const Mat histogram_array = histogram.getArray();
    // Some parameters for drawing
    const int output_width = 640;
    const int output_height = 480;

    const Scalar bin_color(128,128,128);

    int num_bins = histogram_array.rows;
    double max_occurence;
    minMaxLoc(histogram_array, NULL, &max_occurence);

    const double bin_width = 1.0*output_width/num_bins;
    const double bin_height = output_height / (max_occurence);

    histogram_image_ = Mat(output_height, output_width, CV_8UC1);

    Point top_left = Point(0, 0);
    Point bottom_right = Point(0, output_height);

    const float * hist = histogram_array.ptr<float>(0);
    for(int i=0; i<histogram_array.rows; i++){
        top_left.y = bin_height * (max_occurence - hist[i]);
        top_left.x = bin_width * i;
        bottom_right.x = bin_width * (i+1);

        rectangle(histogram_image_, top_left, bottom_right, bin_color);
    }
}
