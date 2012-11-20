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

void zeroMat (Mat &src, Mat &newMat);
float PointsVariance (Point2f center, vector <Point2f> points);
float Distance2d(Point2f &A, Point2f &B);
Point2f GetAveragePoint(vector<Point2f> &nextPoints);
Point2f GetNearestPoint(vector<Point2f> &nextPoints);
