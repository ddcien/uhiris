#include "tracker.h"
//#include <iostream>
#include "differential.h"
#include "utility.h"

#include "highgui.h"

//using std::cout;
//using std::endl;

const double Tracker::myeps = 10e-6;

Tracker::Tracker()
{
	/** Generate filters */
	Differential filters(Differential::SAVGOLMATHWORK);
	filters.ComputeFilterSet();
	h10_ = filters.get_h10();
	h20_ = filters.get_h20();
	h01_ = filters.get_h01();
	h11_ = filters.get_h11();
	h02_ = filters.get_h02();

	/** Empirically set thresholds */
	t_mag_ = 1.4;
	t_ev_ = 1;
	t_ge_ = 0;
}

Tracker::~Tracker() {}

void Tracker::InitializeFrame( Mat input, vector<Point> &eyes )
{
	eyes.clear();

	Mat img, tmp, gray;
	if (input.channels() == 3) {
		cvtColor(input, tmp, CV_RGB2GRAY);
		tmp.convertTo(gray, CV_64FC1);
	}
	else
		input.convertTo(gray, CV_64FC1);

	/**
			By applying Gaussian Blur twice, many false detections (most likely due to noise) can be eliminated.
			However, at the same time we comprised the robustness against pose changes: if the eye is close to the
			facial boundary, it will be smoothed out (blend into the background).
			Applying smoothing only once has the opposite effect. Hence there is a trade-off here.
	*/
	GaussianBlur(gray, tmp, Size(15, 15), 2.5);
	GaussianBlur(tmp, img, Size(15, 15), 2.5);

	Mat f20x, f11xy, f02y, f10x, f01y;
	filter2D(img, f10x, -1, h10_);
	filter2D(img, f20x, -1, h20_);
	filter2D(img, f01y, -1, h01_);
	filter2D(img, f11xy, -1, h11_);
	filter2D(img, f02y, -1, h02_);
	//Dump2DMatrix("f10x.txt", f10x, CV_64FC1);
	//Dump2DMatrix("f20x.txt", f20x, CV_64FC1);
	//Dump2DMatrix("f01y.txt", f01y, CV_64FC1);
	//Dump2DMatrix("f11xy.txt", f11xy, CV_64FC1);
	//Dump2DMatrix("f02y.txt", f02y, CV_64FC1);

	Mat labels(img.rows, img.cols, CV_32FC1);
	Mat hessian(2, 2, CV_64FC1);
	for (int i = 0; i < img.rows; ++i)
		for (int j = 0; j < img.cols; ++j) {
			/** remove small values ... or not */
			double f10, f20, f01, f11, f02;
			//f10x.at<double>(i,j) < myeps ? f10 = 0 : 
				f10 = f10x.at<double>(i,j);
			//f20x.at<double>(i,j) < myeps ? f20 = 0 : 
				f20 = f20x.at<double>(i,j);
			//f01y.at<double>(i,j) < myeps ? f01 = 0 : 
				f01 = f01y.at<double>(i,j);
			//f11xy.at<double>(i,j) < myeps ? f11 = 0 : 
				f11 = f11xy.at<double>(i,j);
			//f02y.at<double>(i,j) < myeps ? f02 = 0 : 
				f02 = f02y.at<double>(i,j);

			Mat eigenvalues, eigenvectors;
			hessian.at<double>(0,0) = f20;
			hessian.at<double>(0,1) = f11;
			hessian.at<double>(1,0) = f11;
			hessian.at<double>(1,1) = f02;
			//PrintMat(&CvMat(hessian));
			eigen(hessian, eigenvalues, eigenvectors);
			Mat grad(1, 2, CV_64FC1);
			grad.at<double>(0, 0) = f10;
			grad.at<double>(0, 1) = f01;
			double eval1 = eigenvalues.at<double>(0,0);
			double eval2 = eigenvalues.at<double>(1,0);
			//PrintMat(&CvMat(eigenvectors));
			Mat evec1 = eigenvectors.row(0);
			//PrintMat(&CvMat(evec1));
			Mat evec2 = eigenvectors.row(1);
			//PrintMat(&CvMat(evec2));
			Topographic label = TopographicClassification(grad, eval1, eval2, evec1, evec2);
			if (label == PIT)
				eyes.push_back(Point(i,j));
			labels.at<float>(i,j) = label;
		}

	CleanUpEyeVector(eyes);

	

	//Dump2DMatrix("labels.txt", labels, CV_32FC1);
	Mat label_map;
	labels.convertTo(tmp, CV_8UC1);
	namedWindow("Label Map", CV_WINDOW_AUTOSIZE);
	equalizeHist(tmp, label_map);
	imshow("Label Map", label_map);


}

Tracker::Topographic Tracker::TopographicClassification( Mat grad, double eval1, double eval2, Mat evec1, Mat evec2 )
{
	double mag = norm(grad);
	if (mag < t_mag_ && eval1 < -1*t_ev_ && eval2 < -1*t_ev_)
		return PEAK;
	else if (mag < t_mag_ && eval1 > t_ev_ && eval2 > t_ev_)
		return PIT;
	else if (mag < t_mag_ && eval1 * eval2 < 0) {
		if (eval1 + eval2 < 0)
			return RIDGESADDLE;
		else return RAVINESADDLE;
	}
	else if ((mag >= t_mag_ && eval1 < -1 * t_ev_ && fabs(grad.dot(evec1)) < t_ge_) ||
			 (mag >= t_mag_ && eval2 < -1 * t_ev_ && fabs(grad.dot(evec2)) < t_ge_) ||
			 (mag < t_mag_ && eval1 < -1 * t_ev_ && fabs(eval2) <= t_ev_) ||
			 (mag < t_mag_ && fabs(eval1) <= t_ev_ && eval2 < -1 * t_ev_))
		return RIDGE;
	else if ((mag >= t_mag_ && eval1 > t_ev_ && fabs(grad.dot(evec1)) < t_ge_) ||
			 (mag >= t_mag_ && eval2 > t_ev_ && fabs(grad.dot(evec2)) < t_ge_) ||
			 (mag < t_mag_ && eval1 > t_ev_ && fabs(eval2) <= t_ev_) ||
			 (mag < t_mag_ && fabs(eval1) <= t_ev_ && eval2 > t_ev_))
		return RAVINE;
	else if (mag < t_mag_ && fabs(eval1) <= t_ev_ && fabs(eval2) <= t_ev_)
		return FLAT;
	else if ((fabs(grad.dot(evec1)) >= t_ge_ && fabs(grad.dot(evec2)) >= t_ge_) ||
			 (fabs(grad.dot(evec1)) >= t_ge_ && fabs(eval2) <= t_ev_) ||
			 (fabs(grad.dot(evec2)) >= t_ge_ && fabs(eval1) <= t_ev_) ||
			 (mag >= t_mag_ && fabs(eval1) <= t_ev_ && fabs(eval2) <= t_ev_)) {
		if (fabs(eval1) <= t_ev_ && fabs(eval2) <= t_ev_)
			return SLOPEHILL;
		else if ((eval1 > 0 && eval2 >= 0) || (eval1 >= 0 && eval2 > 0))
			return CONVEXHILL;
		else if ((eval1 < 0 && eval2 <= 0) || (eval1 <= 0 && eval2 < 0))
			return CONCAVEHILL;
		else if (eval1 * eval2 < 0) {
			if (eval1 + eval2 < 0)
				return CONVEXSADDLEHILL;
			else return CONCAVESADDLEHILL;
		}
	}
	else return UNKNOWN;
}

void Tracker::CleanUpEyeVector( vector<Point> &eyes )
{
	if (eyes.empty()) return;

	vector<Point> clean;
	int ub = eyes.size();
	Mat examined(ub, ub, CV_8UC1, Scalar(0));
	int *mask = new int [ub];
	memset(mask, 0, ub*sizeof(*mask));
	
	for (int i = 0; i < ub; ++i) {
		examined.at<uchar>(i,i) = 1;
		for (int j = 0; j < ub; ++j) {
			if (mask[j] || examined.at<uchar>(i,j))
				continue;
			examined.at<uchar>(i,j) = 1;
			examined.at<uchar>(j,i) = 1;
			if (EuclideanDistance(eyes[i], eyes[j]) < 3)
				mask[j] = 1;
		}
	}

	for (int i = 0; i < ub; ++i)
		if (!mask[i])
			clean.push_back(eyes[i]);

	eyes = clean;

	delete [] mask;
}

void Tracker::Classification( const Mat& labels, vector<Point> &eyes )
{
	if (eyes.empty()) return;

	int num_of_candidates = eyes.size();
	Mat examined(num_of_candidates, num_of_candidates, CV_8UC1, Scalar(0));

	int height = labels.rows;
	int width = labels.cols;

	for (int i = 0; i < num_of_candidates; ++i) {
		Point current = eyes[i];
		examined.at<uchar>(i,i) = 1;
		for (int j = 0; j < num_of_candidates; ++j) {
			if (examined.at<uchar>(i,j))
				continue;
			examined.at<uchar>(i,j) = 1;
			examined.at<uchar>(j,i) = 1;

			Point target = eyes[j];
			float dist = EuclideanDistance(current, target);
			/** arguable thresholds for different face scales */
			if (dist < 120 && dist > 40) {
				float theta = atan2(static_cast<float>(current.x-target.x), static_cast<float>(current.y-target.y));
				/** patch size*/
				int rect_w = static_cast<int>(0.6 * dist + 0.5);
				int rect_h = static_cast<int>(0.3 * dist + 0.5);

			}
		}
	}
}

float Tracker::EuclideanDistance( const Point& p1, const Point& p2 )
{
	return sqrt(pow(static_cast<float>(p1.x - p2.x), 2) + pow(static_cast<float>(p1.y - p2.y), 2));
}