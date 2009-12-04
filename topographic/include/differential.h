#ifndef _DIFFERENTIAL_H_
#define _DIFFERENTIAL_H_
#include "cv.h"

using namespace cv;

class Differential {

public:
	enum FilterType {CHEBYSHEV, SAVGOLMATHWORK, SAVGOLMICROSOFT};
	Differential();
	Differential(FilterType type);
	Differential(FilterType type, int halfspan);
	~Differential();
	bool ComputeFilterSet();

	Mat get_h10() { return h10_; }
	Mat get_h01() { return h01_; }
	Mat get_h11() { return h11_; }
	Mat get_h20() { return h20_; }
	Mat get_h02() { return h02_; }
	FilterType get_type() { return type_; }
	int get_halfspan() { return halfspan_; }

private:
	Mat ChebyshevZero(int N);
	Mat ChebyshevFirst(int N);
	Mat ChebyshevSecond(int N);

	static const float mwSavGolSize5Order1X1Y0_[25];
	static const float mwSavGolSize5Order1X0Y1_[25];
	static const float mwSavGolSize5Order2X1Y1_[25];
	static const float mwSavGolSize5Order2X2Y0_[25];
	static const float mwSavGolSize5Order2X0Y2_[25];

	static const float msSavGolSize5Order1X1Y0_[25];
	static const float msSavGolSize5Order1X0Y1_[25];
	static const float msSavGolSize5Order2X1Y1_[25];
	static const float msSavGolSize5Order2X2Y0_[25];
	static const float msSavGolSize5Order2X0Y2_[25];

	Mat h10_;
	Mat h01_;
	Mat h11_;
	Mat h20_;
	Mat h02_;
	FilterType type_;
	int halfspan_;

};

#endif