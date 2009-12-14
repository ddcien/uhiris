#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include "cv.h"
#include "highgui.h"
#include "tracker.h"

#include "differential.h"
#include "utility.h"

using std::cout;
using std::endl;
using std::ofstream;
using std::vector;
using namespace cv;

int main(int argc, char** argv) {

	Tracker tracker;
	vector<Point> eyes;
		
	VideoCapture cap(0);
	if (!cap.isOpened()) return -1;
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	//VideoWriter video_writer("out_frame_big5.avi", -1, 5, Size(640, 480), true);
	//if (!video_writer.isOpened()) return -1;
	
	namedWindow("Input", CV_WINDOW_AUTOSIZE);

	Mat rgb;
	cap >> rgb;
	//rgb = imread("out_frame_big2_100.jpg");
	tracker.InitializeFrame(rgb, eyes);
	int num_of_candidates = eyes.size();
	for (int i = 0; i < num_of_candidates; ++i)
		circle(rgb, Point(eyes[i].y,eyes[i].x), 5, CV_RGB(0,255,0));
	imshow("Input", rgb);
	//waitKey();

	for (;;) {

		cap >> rgb;
		//tracker.TrackEyes(rgb, eyes);
		tracker.InitializeFrame(rgb, eyes);
		int num_of_candidates = eyes.size();
		for (int i = 0; i < num_of_candidates; ++i)
			circle(rgb, Point(eyes[i].y,eyes[i].x), 5, CV_RGB(0,255,0));
		imshow("Input", rgb);
		//video_writer << rgb;
		
		if (waitKey(30) >= 0) break;
	}

	return 0;
}
