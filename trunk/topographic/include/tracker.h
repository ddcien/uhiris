#ifndef _TRACKER_H_
#define _TRACKER_H_
#include <vector>
#include "cv.h"
#include "svm.h"

using namespace cv;
using std::vector;

class Tracker {

public:
	enum Topographic {UNKNOWN, PEAK, PIT, RIDGE, RAVINE, RIDGESADDLE, RAVINESADDLE, CONVEXHILL, CONCAVEHILL, CONVEXSADDLEHILL, CONCAVESADDLEHILL, SLOPEHILL, FLAT};
	Tracker();
	~Tracker();

	/**
		If no iris is located via Mutual Information search, try reinitialize.
		Keep initializing until the iris is located.
	*/
	void InitializeFrame(Mat input, const Point& reference, bool accumu, vector<Point> &eyes);
	void InitializeFrame(Mat input, const Point& reference, vector<Point> &eyes);
	void InitializeFrame(Mat input, vector<Point> &eyes);
	void TrackEyes(Mat input, vector<Point> &eyes);

private:
	Topographic TopographicClassification(Mat grad, double eval1, double eval2, Mat evec1, Mat evec2);
	void CleanUpEyeVector(vector<Point> &eyes);
	void Classification( const Mat& labels, vector<Point> &eyes);
	float EuclideanDistance(const Point& p1, const Point& p2);
	Mat GetPatch( const Mat& whole, const Point& center, float dist, float theta);
	Mat GetHistogram( const Mat& input, CvHistogram* hist);
	bool CheckLIBSVM( const Mat& hist);
	bool CheckSVMLIGHT( const Mat& hist);
	Mat GenerateLabelMap(Mat input, vector<Point> &pits);

	float t_mag_;
	float t_ev_;
	float t_ge_;

	Mat h10_;
	Mat h20_;
	Mat h01_;
	Mat h11_;
	Mat h02_;

	svm_model* svm_model_;
	svm_node* svm_nodes_;

	int svmlight_model_;

	static const double myeps_;
};

#endif