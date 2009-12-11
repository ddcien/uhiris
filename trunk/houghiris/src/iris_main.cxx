#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;
using namespace cv;

int main(int argc, char ** argv){

    // PARAMETERS
    const string cascade_name = "haarcascade_lefteye_2splits.xml" ;
    const int max_frame = 30;
    const double edge_threshold = 150;
    const double hough_circle_min_distance = 10;

    int count_frame = 0;

    if (argc < 2){
        printf("usage: %s filename.wmv\n", argv[0]);
        exit(1);
    }

    // read this video
    VideoCapture capture(argv[1]);
    Mat temp_frame; // temporary store the currently processed frame
    Mat temp_out_frame;
    Mat cropped_frame;
    Mat cropped_frame_gray;
    Mat thresholded_image;
    Mat thresholded_image2;
    Mat edges;
    vector<Rect> detected_objects;
    vector<Vec3f> circles; // hough circles
    CvAdaptiveSkinDetector skin_detector;

    char * debug_window = "Cropped Frame";
    namedWindow(debug_window, CV_WINDOW_AUTOSIZE);

    // load eye cascade
    CascadeClassifier cascade(cascade_name);

    if( cascade.empty() )
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return -1;
    }

    // Capture first frame
    capture >> temp_frame;

    // write out detected eye cascade to the 2nd video
    VideoWriter video_writer("out_frame.avi", CV_FOURCC('M','P','E','G'), 30, temp_frame.size(), true);

    IplImage * cropped_frame_ipl;
    IplImage * thresholded_image_ipl;

    // while we can still capture
    while (count_frame<max_frame && capture.grab()){
        //while (capture.grab()){
        capture.retrieve(temp_frame);

        // Perform detection
        cascade.detectMultiScale( temp_frame, detected_objects,
            1.1, 2, 0
            |CV_HAAR_FIND_BIGGEST_OBJECT
            //|CV_HAAR_DO_ROUGH_SEARCH
            |CV_HAAR_SCALE_IMAGE
            ,
            Size(30, 30) );

        // draw this object
        temp_out_frame = temp_frame.clone();

		Mat eroded_mask;
		Mat masked_edges;

        if (detected_objects.size() > 0 ){
			// we only take the bottom part => eye only no eye brow
			Rect eye_only_rectangle(
				detected_objects[0].tl().x, detected_objects[0].tl().y + detected_objects[0].height*0.4,
				detected_objects[0].width,detected_objects[0].height*0.6);

            rectangle(temp_out_frame, eye_only_rectangle.tl(), eye_only_rectangle.br(), CV_RGB(255,0,0));
            cropped_frame = Mat(temp_frame, eye_only_rectangle);

            // only use gray image (since Canny can only handle gray)
            cvtColor(cropped_frame, cropped_frame_gray, CV_RGB2GRAY);

            // threshold image to remove skin locations
			// this step create a black and white image (white is the part to keep)
            threshold(cropped_frame_gray, thresholded_image, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

			// erode the white part
			erode(thresholded_image, eroded_mask, Mat());

			imshow(debug_window, thresholded_image);
			waitKey(0);
			imshow(debug_window, eroded_mask);
			waitKey(0);
            
			// NOTE: the problem with thresholding is that it creates very strong edge. When we create edges, we need a way to remove
            //       those that are created by this step
			// SOLUTION: we perform thresholding, then keep the thresholded image as a mask. We erode the white part (the mask is a bit bigger)
			//           then we apply this mask on the detected edges

            // canny edge detection
            edges = Mat(cropped_frame_gray.size(),cropped_frame_gray.type());
            Canny(cropped_frame_gray, edges, edge_threshold, edge_threshold * 3);

			imshow(debug_window, edges);
			waitKey(0);

			// apply eroded mask on edges to remove edges in sections we do not care about (skin)
			masked_edges = edges.mul(1.0/255 * eroded_mask);

			imshow(debug_window, masked_edges);
			waitKey(0);

            // hough circle
            HoughCircles(edges, circles, CV_HOUGH_GRADIENT, 1, hough_circle_min_distance, 1, 10);

            for(size_t i=0; i <circles.size();i++)
            {
                Point center(cvRound(circles[i][0]),cvRound(circles[i][1]));
                int radius = cvRound(circles[i][2]);
                //draw the circle center
                circle(cropped_frame, center , 3 , CV_RGB(255,0,0),1,8,0);
                //draw the circle out line
                circle(cropped_frame, center, radius, CV_RGB(0,0,255),1,8,0);
            }
        }

        printf(".");

        char out_filename[200];
        char out_iris_filename[200];
        char out_edge_filename[200];
        sprintf(out_filename, "input_%d.png", count_frame);
        sprintf(out_edge_filename, "edge_%d.png", count_frame);
        sprintf(out_iris_filename, "iris_%d.png", count_frame);
        cvSaveImage(out_filename, &IplImage(thresholded_image));
        cvSaveImage(out_edge_filename, &IplImage(masked_edges));
        cvSaveImage(out_iris_filename, &IplImage(cropped_frame));
        video_writer << temp_out_frame;

        // NOTE: for now, don't process the entire thing yet
        count_frame++;
    }

    // Done
    std::cout << "Done" << endl;
    return 1;

}
