#include "Drawing.h"
#include "ImageUtils.h"
#include "MathUtils.h"
#include <stdio.h>
#include <stdlib.h>


using namespace cv;
using namespace std;

// calculates variance of points from the center
float PointsVariance (Point2f center, vector <Point2f> points)
{
	float sum = 0.0f;
	vector<float> distances;
	for(int i = 0; i<points.size(); i++)
	{
		distances.push_back(Distance2d(center,points[i]));
		sum += distances[i];
	}
	float averageDist = sum/points.size();
	sum = 0.0f;
	for(int i=0;i<points.size();i++)
		sum += (distances[i] - averageDist) * ( distances[i] - averageDist);
	return sum / (points.size()*1000);

}

//calculates distance between 2 points
float Distance2d(Point2f &A, Point2f &B) {
	return (A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y);
}

//making zero Mat
void zeroMat (Mat &src, Mat &newMat)
{
		newMat = Mat::zeros(src.rows, src.cols, src.type());
}


//Calculating average of points
Point2f GetAveragePoint(vector<Point2f> &nextPoints) {
	Point2f average = Point2f(0.0f, 0.0f);
	for(int i=0;i<nextPoints.size();i++) {
		average.x += nextPoints[i].x;
		average.y += nextPoints[i].y;
	}
	average.x /= (float)nextPoints.size();
	average.y /= (float)nextPoints.size();
	
	return average;
}

//Finding point nearest to average
Point2f GetNearestPoint(vector<Point2f> &nextPoints) {
	Point2f average = GetAveragePoint(nextPoints);

	const float maxDistance = 10000.0f;

	vector<Point2f> closePoints;
	for(int i=0;i<nextPoints.size();i++)
		if (Distance2d(nextPoints[i], average) < maxDistance)
			closePoints.push_back(nextPoints[i]);
	Point2f averageFromGoodPoints = GetAveragePoint(closePoints);
	
	vector<float> mininumDistances;
	for(int i=0;i<nextPoints.size();i++)
		mininumDistances.push_back(Distance2d(nextPoints[i], averageFromGoodPoints));
	
	
	int index=0;
	int minValue = mininumDistances[0];
	for(int i=1;i<nextPoints.size();i++)
		if (mininumDistances[i] < minValue) {
			minValue = mininumDistances[i];
			index = i;
		}

	return nextPoints[index];

}


