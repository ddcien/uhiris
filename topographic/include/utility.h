#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "cv.h"

using namespace cv;

void PrintMat(CvMat *A);
void Dump2DMatrix(const char* filename, const Mat& mat, int type);

#endif