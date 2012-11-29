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

void DrawHough(Mat &src, vector<Vec4i> &lines);
void DrawOpticalFlow (Mat& src, vector<Point2f>& prevPoints, vector<Point2f>& nextPoints);
void DrawHoughCircles ( Mat &dst, vector<Vec3f> circles);
void DrawCentralPoint ( Mat &dst, Point center);
