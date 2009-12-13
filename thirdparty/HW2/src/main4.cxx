#include "Misc.h"
#include <opencv/highgui.h>
#include <iostream>

Point center;
Mat image, logpolar_image, log_retinal_image, retinal_image, logpolar_image_repeat_x;
const char * retinal_image_window = "Retinal Image";
const char * log_retinal_image_window = "Log Retinal Image";
const char * polar_image_window = "Polar Image";
const char * logpolar_image_repeat_x_window = "Log Polar Image Repeat X";
int size_theta, size_rho;

void computeRetinalImage(){
    logpolar_image = dc::logPolar(image, size_rho, size_theta, center);

	// we pad the right side of our logpolar_image with the first column so interpolation will work as if it is repeating.
	logpolar_image_repeat_x = Mat(logpolar_image.rows, logpolar_image.cols+1, CV_8U);
        Mat target_logpolar_image_region = Mat(logpolar_image_repeat_x, Rect(0,0,logpolar_image.cols, logpolar_image.rows));
        logpolar_image.copyTo( target_logpolar_image_region ) ;

        Mat logpolar_image_repeat_x_last_column = logpolar_image_repeat_x.col(logpolar_image.cols);
        logpolar_image.col(0).copyTo(logpolar_image_repeat_x_last_column);

	Mat target_point = Mat(2,1,CV_32F);

    // perform inverse mapping for retinal image
    // rho = e^R - a
    // theta = TH

// 	log_retinal_image = Mat(logpolar_image_repeat_x.size(), CV_8U);
//     for (int R=0; R<log_retinal_image.rows; R++){
//         for (int TH=0; TH<log_retinal_image.cols; TH++){
//             target_point.at<float>(0,0) = TH;//log(double(R)) - 0.3; //rho
// 			//target_point.at<float>(1,0) = (R); // theta
//             target_point.at<float>(1,0) = log(double(R))/log(1.1) ; // theta
// 
//             log_retinal_image.at<uchar>(R,TH) = dc::bilinear_interpolate(logpolar_image_repeat_x, target_point);
//         }
//     }
//     cvSaveImage("log_retinal_image.png", &IplImage(log_retinal_image));

    // perform inverse mapping from log retinal image to x,y retinal image
    // R = sqrt(x*x + y*y)
    // TH = atan(y/x)
    retinal_image = Mat(logpolar_image_repeat_x.rows*2, logpolar_image_repeat_x.rows*2, CV_8U);
    float theta_step_per_radian = 1.0 * logpolar_image.cols / (2.0 * dc::pi);
    int centered_x, centered_y;
    double temp_arctan = 0;
    float temp_theta = 0;
    for (int y=0; y<retinal_image.rows; y++){
        for (int x=0; x<retinal_image.cols; x++){
            centered_x = x - center.x;
            centered_y = y - center.y;
            //target_point.at<float>(1,0) = pow(1.03, sqrt(double(centered_x*centered_x + centered_y*centered_y))) - 0.3; // R
            target_point.at<float>(1,0) = exp(sqrt(double(centered_x*centered_x + centered_y*centered_y))) -0.3; // R

            temp_arctan = atan2(double(centered_y),double(centered_x));
            temp_theta = (temp_arctan>0 ? temp_arctan : temp_arctan + 2*dc::pi) * theta_step_per_radian;
            target_point.at<float>(0,0) = temp_theta; //TH

            //std::cout << target_point.at<float>(1,0) << " " << target_point.at<float>(0,0) << endl;
//             if (centered_x == 64 && centered_y == -2){
//                 std::cout << target_point.at<float>(1,0) << " " << target_point.at<float>(0,0) << endl;
//             }

            retinal_image.at<uchar>(y,x) = dc::bilinear_interpolate(logpolar_image_repeat_x, target_point);
        }
    }

    // update visualization
    //imshow(log_retinal_image_window, log_retinal_image);
    imshow(retinal_image_window, retinal_image);
}

void mouseCallback(int event, int x, int y, int flags, void* param){
    if (event == CV_EVENT_LBUTTONDOWN){
        center = Point(x,y);
        computeRetinalImage();
    }
}

int main (int argc, char** argv){
    if (argc <3){
            printf("usage: %s input_image.png 64\n", argv[0]);
            exit(1);
    }

    image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	// sanity check
	if (!image.data){
		std::cerr << "Cannot load file: " << argv[1] << endl;
		exit(-1);
	}

    center = Point(image.cols/2, image.rows/2);
    // calculate output size
    size_theta = atoi(argv[2]);
    size_rho = image.cols / 2;
    logpolar_image = dc::logPolar(image, size_rho, size_theta, center);
    
    computeRetinalImage();

    const char * original_image_window = "Original";
    namedWindow(original_image_window, CV_WINDOW_AUTOSIZE);
    imshow(original_image_window, image);

    namedWindow(log_retinal_image_window, CV_WINDOW_AUTOSIZE);
//    imshow(log_retinal_image_window, log_retinal_image);

	namedWindow(polar_image_window, CV_WINDOW_AUTOSIZE);
	imshow(polar_image_window, logpolar_image);

    namedWindow(retinal_image_window, CV_WINDOW_AUTOSIZE);
    imshow(retinal_image_window, retinal_image);

	namedWindow(logpolar_image_repeat_x_window, CV_WINDOW_AUTOSIZE);
	imshow(logpolar_image_repeat_x_window, logpolar_image_repeat_x);

    cvSetMouseCallback( original_image_window, mouseCallback, 0);

    waitKey(0);
	cvSaveImage("retinal_image.png", &IplImage(retinal_image));

    return 1;
}
