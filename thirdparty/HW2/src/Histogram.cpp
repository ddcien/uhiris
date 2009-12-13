#include "Histogram.h"

/** \brief Create a 1-channel 256-value histogram from a 1-channel image
  \param image 1-channel grayscale image
  \return A normalized histogram of the input image

  \todo Test that this method will work with ROI
*/
dc::Histogram dc::Histogram::fromImage( const Mat & image )
{
	if(image.channels() > 1){
		throw std::invalid_argument("Only accepting 1-channel grayscale image");
	}
	if (image.rows == 0 && image.cols == 0){
		throw std::invalid_argument("Input image is empty");
	}

	Mat histogram_array = Mat::zeros(Size(1,256), CV_32FC1);
	float * hist = histogram_array.ptr<float>(0);
	const double normalize_ratio = 1.0/(image.cols * image.rows);

	MatConstIterator_<uchar> it = image.begin<uchar>(),
		it_end = image.end<uchar>();
	for(; it != it_end; ++it)
	{
		hist[*it] += normalize_ratio; // 1 * normalize_ratio

	}

	std::cout << "Histogram values: ";
	for (int i=0; i<256; i++){
		std::cout << hist[i] <<  " ";
	}
	std::cout << std::endl;
	return Histogram(histogram_array);
}

const Mat dc::Histogram::getArray () const
{
	return histogram_array_;
}

dc::Histogram::Histogram( Mat histogram_array )
{
	histogram_array_ = histogram_array;
}

dc::Histogram::Histogram()
{
	// BECAREFUL: User has not initialized anything
	// We could have added checks so that if a user call these methods
	// but that would unnecessary slow down our method calls, user will know
	// that they have an error when they try to use getArray()
}

dc::Histogram::~Histogram()
{

}

/** \brief Create a Histogram that looks like a triangle
*/
dc::Histogram dc::Histogram::createTriangleHistogram()
{
	// cache this triangle histogram so the next call will not create it again
	static Mat triangle_histogram;
	
	if (triangle_histogram.empty()){
		triangle_histogram = Mat(256,1, CV_32F);
	} else {
		return triangle_histogram;
	}
		
	// we go from 0 at 0 to 127 at 127 then 127 at 128 to 0 at 255
	// Thus we have 128 pairs that always add up to 127
	double normalize_ratio = 1.0 / (127*128);

	// populate left half
	for (int i=0; i<128; i++)
		triangle_histogram.at<float>(i,0) = i * normalize_ratio;

	// populate right half
	for (int i=128; i<256; i++)
		triangle_histogram.at<float>(i,0) = (255-i) * normalize_ratio;

	return triangle_histogram;
}

dc::Histogram dc::Histogram::createGaussianHistogram( float mean /*= 128*/, float sigma /*= 32*/ )
{
	static Mat gaussian_histogram;
	if (gaussian_histogram.empty()){
		gaussian_histogram = Mat(256,1, CV_32F);
	} else {
		return gaussian_histogram;
	}

	// we go from 0 at 0 to 127 at 127 then 127 at 128 to 0 at 255
	// Thus we have 128 pairs that always add up to 127
	double normalize_ratio = 1.0 / (127*128);

	// populate left half
	const float gaussian_ratio = 1.0 / (sigma * sqrt(2*pi));
	const float inv_two_sigma_square = -1 / (2 * sigma * sigma);
	
	for (int i=0; i<256; i++)
		gaussian_histogram.at<float>(i,0) = gaussian_ratio * exp(inv_two_sigma_square * pow(i-mean,2));

	return gaussian_histogram;
}

/** \brief Shape this histogram to another histogram

	\param another_histogram target histogram to shape to
	\param image apply the new intensity value map to this image
	
	\return The input image with its intensity mapped by the histogram shaping operation

	\note Only support 256 intensity value
*/
cv::Mat dc::Histogram::shapeTo( const Histogram & another_histogram, const Mat & image )
{
	// STEP 0: Ready return value
	Mat new_image = image.clone();

	// STEP 1: Find cdf for these histogram
	Mat this_cdf = computeCDFFromPDF(histogram_array_);
	Mat other_cdf = computeCDFFromPDF(another_histogram.getArray());

	// STEP 2: Match this cdf with the lowest possible other cdf point which has the same or a bit larger
	Mat new_intensity = Mat(256,1,CV_8U);
	float cdf_to_match;
	for (int i=0; i<this_cdf.rows; i++){
		cdf_to_match = this_cdf.at<float>(i,0);

		// search in other_cdf for this cdf
		int j = 0;
		while(cdf_to_match > other_cdf.at<float>(j,0)){
			if (j+1 >= other_cdf.rows)
				break;

			j++;
		}
		new_intensity.at<uchar>(i,0) = j;
		printf("%d => %d, ", i, j);
	}
	std::cout << std::endl;

	// STEP 2: apply intensity map onto the image
	uchar old_intensity;
	for (int y=0; y<image.rows; y++){
		for (int x=0; x<image.cols; x++){
			old_intensity = image.at<uchar>(y,x);
			new_image.at<uchar>(y,x) = new_intensity.at<uchar>( old_intensity ,0);
		}
	}
	
	return new_image;
}

/** \brief Compute CDF from an input Nx1 PDF vector
	Only work with 1 channel PDF
*/
Mat dc::Histogram::computeCDFFromPDF(const Mat & histogram_array){
	if (histogram_array.channels() > 1)
		throw std::invalid_argument("Only handle 1-channel pdf matrix");

	Mat cdf = histogram_array.clone();
	float sum=0;
	for (int i=0; i<histogram_array.rows; i++){
		sum += histogram_array.at<const float>(i,0);
		cdf.at<float>(i,0) = sum;
//		std::cout << i << " => " << sum << std::endl;
	}

	return cdf;
}