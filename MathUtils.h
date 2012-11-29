#include "stdafx.h"

using namespace cv;
using namespace std;

void zeroMat (Mat &src, Mat &newMat);
float PointsVariance (Point2f center, vector <Point2f> points);
float Distance2d(Point2f &A, Point2f &B);
Point2f GetAveragePoint(vector<Point2f> &nextPoints);
Point2f GetNearestPoint(vector<Point2f> &nextPoints);
Point2f GetNearestPoint(vector<Point2f> &points, Point2f center);
vector<Point2f> FindMaskPoints(Mat & mask);
bool VectorContains (vector<Point2f> points, Point2f point);
Scalar GetColor (Mat &src,Point point);
