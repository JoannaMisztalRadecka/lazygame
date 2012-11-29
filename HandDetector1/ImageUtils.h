#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "math.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <sstream>
#include <stdlib.h>

using namespace cv;
using namespace std;

void ConvertToGray(Mat& src, Mat& src_gray);
void FindPointsGFT(Mat& src,vector<Point2f>& corners, Mat& mask);
void OpticalFlow(Mat& src1, Mat& src2, vector<Point2f>& prevPoints, vector<Point2f>& nextPoints,
				 vector<uchar>& statusVector, vector<float>& errorVector);
void pickPoint (Mat &src,MatND &dst, Mat &mask, Point seed, int lo, int up );
void HistAndBackproj(Mat &hsv, Mat &mask, MatND &dst );
void ConvertToHSV(Mat &src, Mat &dst);
void HistNormalize(Mat &hist);
