#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;
using namespace cv;

int main(int argc, char ** argv){
	const string cascade_name = "haarcascade_lefteye_2splits.xml" ;
	const int max_frame = 100;

	int count_frame = 0;

	if (argc < 2){
		printf("usage: %s filename.wmv\n");
		exit(1);
	}

	// read this video
	VideoCapture capture(argv[1]);
	Mat temp_frame; // temporary store the currently processed frame
	Mat temp_out_frame;
	Mat cropped_frame;
	vector<Rect> detected_objects;

	char * cropped_frame_window = "Cropped Frame";
	namedWindow(cropped_frame_window, CV_WINDOW_AUTOSIZE);

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

	// while we can still capture
	while (count_frame<max_frame && capture.grab()){
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

		if (detected_objects.size() > 0 ){
			rectangle(temp_out_frame, detected_objects[0].tl(), detected_objects[0].br(), CV_RGB(255,0,0));
			cropped_frame = Mat(temp_frame, detected_objects[0]);
		}

//		imshow(cropped_frame_window, cropped_frame);
//		waitKey(0);

		printf(".");
		
		char out_filename[200];
		sprintf(out_filename, "left_eye_%d.png", count_frame);
		cvSaveImage(out_filename, &IplImage(cropped_frame));
		video_writer << temp_out_frame;
		
		// NOTE: for now, don't process the entire thing yet
		count_frame++;
	}

	// Done
	std::cout << "Done" << endl;
	return 1;

}