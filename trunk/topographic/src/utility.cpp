#include "utility.h"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::ofstream;

void PrintMat(CvMat *A)
/** http://blog.weisu.org/2007/11/opencv-print-matrix.html */
{
	int i, j;
	for (i = 0; i < A->rows; i++)
	{
		printf("\n");
		switch (CV_MAT_DEPTH(A->type))
		{
		case CV_32F:
		case CV_64F:
			for (j = 0; j < A->cols; j++)
				printf ("%8.3f ", (float)cvGetReal2D(A, i, j));
			break;
		case CV_8U:
		case CV_16U:
			for(j = 0; j < A->cols; j++)
				printf ("%6d",(int)cvGetReal2D(A, i, j));
			break;
		default:
			break;
		}
	}
	printf("\n");
}

void Dump2DMatrix( const char* filename, const Mat& mat, int type )
{
	if (mat.channels() > 1) {
		cout << "Could not handle multi-channel matrices." << endl;
		return;
	}
	if (type != CV_8UC1 && type != CV_32FC1 && type != CV_64FC1) {
		cout << "Could not handle current matrix type, supported types are: CV_8UC1, CV_32FC1 and CV_64FC1." << endl;
		return;
	}

	ofstream fout;
	fout.open(filename, ofstream::out);
	if (fout.fail()) {
		cout << "Could not open " << filename << " for writing." << endl;
		return;
	}

	for (int i = 0; i < mat.rows; ++i) {
		for (int j = 0; j < mat.cols; ++j) {
			switch (type) {
				case CV_8UC1:
					fout << mat.at<uchar>(i,j) << " ";
					break;
				case CV_32FC1:
					fout << mat.at<float>(i,j) << " ";
					break;
				case CV_64FC1:
					fout << mat.at<double>(i,j) << " ";
					break;
			}
		}
		fout << endl;
	}

	fout.close();
}