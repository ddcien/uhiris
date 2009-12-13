#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <fftw3.h>

#include "HistogramImage.h"
#include "Misc.h"
using namespace cv;

Mat create_toeplitz_stripe_image(){
    Mat toeplitz = Mat(48,48,CV_8U, CV_RGB(64,64,64));

    rectangle(toeplitz,Point(8,8), Point(40,40), CV_RGB(255,255,255), CV_FILLED);
    for (int i = 0; i < 16 ; i++)
    {
        line(toeplitz, Point(-48 + 48/8*i,0), Point(48/8*i,48), CV_RGB(255,255,255), 1);
    }

    return toeplitz;

}

/** \brief Perform weighted mask on individual channels of dft input
*/
void apply_two_layer_mask(const char * mask_file_0, const char * mask_file_1, int num_row, int num_col , fftw_complex* dft){
	double mask_0,mask_1;
	ifstream mask_0_fs(mask_file_0);
	ifstream mask_1_fs(mask_file_1);

	for (int i = 0; i < num_row ; i++)
	{
		for (int j = 0; j < num_col ; j++)
		{
			mask_0_fs >> mask_0;
			mask_1_fs >> mask_1;
			
			dft[i*num_col + j][0] *= mask_0;
			dft[i*num_col + j][1] *= mask_1;
		}
	}

	mask_0_fs.close();
	mask_1_fs.close();
}

/** \brief Blank out points on DFT where mask is != 255
*/
void apply_mask(const Mat & mask, fftw_complex* dft){
    int step = mask.cols;
    int count_mask_point = 0;
    for(int i=0; i<mask.rows; i++){
        for(int j=0; j<mask.cols; j++){
            if (mask.at<uchar>(i,j) != 255){
                count_mask_point ++;
                dft[i*step+j][0] = 0;
                dft[i*step+j][1] = 0;
            }
        }
    }
    printf("Total # of mask pixels: %d\n",count_mask_point);
}

int main(int argc, char** argv){
    Mat toeplitz_image = create_toeplitz_stripe_image();
    //Mat rotated_toeplitz_image = toeplitz_image.clone();
    Mat rotated_toeplitz_image = Mat(48,48, CV_8U);

    Mat toeplitz_mat = Mat(toeplitz_image.size(), CV_64F); // the double precision version of the toeplitz_image

    // Create windows
    string toeplitz_window = "Toeplitz Stripe Image";
    string rotated_toeplitz_window = "Rotated Toeplitz Stripe Image";
    namedWindow(toeplitz_window, CV_WINDOW_AUTOSIZE);
    namedWindow(rotated_toeplitz_window, CV_WINDOW_AUTOSIZE);

    // Rotate my image
    dc::rotate(toeplitz_image, rotated_toeplitz_image, Point(toeplitz_image.rows/2, toeplitz_image.cols/2), dc::RADIAN_PER_DEGREE * -45);

    // Show images
    imshow(toeplitz_window,toeplitz_image);
    imshow(rotated_toeplitz_window, rotated_toeplitz_image);

    // save rotated toeplitz image
    cvSaveImage("rotated_toeplitz.png", &IplImage(rotated_toeplitz_image));

    // convert 8U to 64S for FFTW
    rotated_toeplitz_image.convertTo(toeplitz_mat, CV_64F);

    //cvSave("toeplitz_mat.cvmat", &CvMat(toeplitz_mat));

    // cache number of elements in an image (aka num pixels)
    int num_elements = toeplitz_mat.rows * toeplitz_mat.cols;

    // allocate memory for input and output of fftw
    fftw_complex* dft_in = (fftw_complex*) fftw_malloc( num_elements * sizeof(fftw_complex));
    fftw_complex* dft_out = (fftw_complex*) fftw_malloc( num_elements * sizeof(fftw_complex));

    // copy data from cv::Mat to dft_in
    for (int i=0; i< toeplitz_mat.rows; i++)
        for (int j=0; j<toeplitz_mat.cols; j++){
        dft_in[48*i+j][0] = toeplitz_mat.at<double>(i,j);
        dft_in[48*i+j][1] = 0;
    }

    // create an fftw_plan
    fftw_plan dft_forward_plan = fftw_plan_dft_2d(toeplitz_mat.rows, toeplitz_mat.cols, dft_in, dft_out, FFTW_FORWARD, FFTW_ESTIMATE);

    // execute this plan
    fftw_execute(dft_forward_plan);

    // compute magnitude and phase from ouput real and imaginary of dft
    Mat dft_magnitude = Mat(toeplitz_mat.size(), CV_64F);
    Mat dft_phase = Mat(toeplitz_mat.size(), CV_64F);
	
	FILE * dft_output_file = fopen("dft_output.txt", "w");
    
	double temp_real, temp_imaginary;
    int temp_step = dft_magnitude.cols;
    for (int i=0; i<dft_magnitude.rows; i++){
        for (int j=0; j<dft_magnitude.cols; j++){
            temp_real = dft_out[temp_step*i+j][0];
            temp_imaginary = dft_out[temp_step*i+j][1];

			// copy output for saving
			fprintf(dft_output_file, "%g %g\n", temp_real, temp_imaginary);

            dft_magnitude.at<double>(i,j) = sqrt(temp_real*temp_real + temp_imaginary*temp_imaginary);
            dft_phase.at<double>(i,j) = atan(temp_imaginary/temp_real);
        }
    }

	fclose(dft_output_file);

    cvSaveImage("dft_magnitude.png", &IplImage(dft_magnitude));
    cvSaveImage("dft_phase.png", &IplImage(dft_phase));

    // Logarithmically compress + contrast stretch magnitude and phase
    Mat compress_magnitude;

    log(dft_magnitude, compress_magnitude);
    compress_magnitude = dc::contrast_stretch_to_image(compress_magnitude);
    cvSaveImage("compressed_magnitude_noshift.png", &IplImage(compress_magnitude));
    compress_magnitude = dc::fftshift(compress_magnitude);
    cvSaveImage("compressed_magnitude.png", &IplImage(compress_magnitude));

    Mat compress_phase;
    log(dft_phase, compress_phase);
    compress_phase = dc::contrast_stretch_to_image(compress_phase);
    compress_phase = dc::fftshift(compress_phase);
    cvSaveImage("compressed_phase.png", &IplImage(compress_phase));

    // Display resulting magnitude and phase
    namedWindow("Magnitude", CV_WINDOW_AUTOSIZE);
    namedWindow("Phase", CV_WINDOW_AUTOSIZE);

    imshow("Magnitude", compress_magnitude);
    imshow("Phase", compress_phase);

    waitKey(0);

    // Load mask
    Mat magnitude_mask = imread("dft_magnitude_mask.png",CV_LOAD_IMAGE_GRAYSCALE);

    // Apply mask
    // NOTE: dft_out is the output of DFT (not stretched or shifted)
    //       Using fftshift as unshift ONLY work when the input size NxN has N even.
    
	//Mat shifted_magnitude_mask = dc::fftshift(magnitude_mask);
    //apply_mask(shifted_magnitude_mask, dft_out);

	//apply_mask(magnitude_mask, dft_out);
	apply_two_layer_mask("dft_mask_0.txt", "dft_mask_1.txt", 48,48, dft_out);

    fftw_complex * idft = (fftw_complex*) fftw_malloc(num_elements * sizeof(fftw_complex));
    // Perform invert FFT on masked out frequency image
    fftw_plan dft_backward_plan = fftw_plan_dft_2d(dft_magnitude.rows, dft_magnitude.cols, dft_out, idft, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(dft_backward_plan);

    // copy out results from the real channel
    Mat new_image = Mat(dft_magnitude.size(), CV_8UC1);
    for (int i=0; i<dft_magnitude.rows; i++){
        for (int j=0; j<dft_magnitude.cols; j++){
            new_image.at<uchar>(i,j) = 1.0/(48*48) * idft[dft_magnitude.cols * i + j][0];
        }
    }

	// TODO: contrast stretch this image

    cvSaveImage("new_image.png", &IplImage(new_image));

    // free memory
    fftw_free(idft);
    fftw_free(dft_in);
    fftw_free(dft_out);
    fftw_destroy_plan(dft_forward_plan);
    fftw_destroy_plan(dft_backward_plan);

    return 1;
}
