#include "Drawing.h"
#include "MathUtils.h"
#include "ImageUtils.h"
#include "HandDetector.h"
#include <stdio.h>
#include <stdlib.h>


using namespace cv;
using namespace std;


	//making initial mask
void HandDetector::Calibration()
	{
		VideoCapture cap(0); 
		// open the default camera
		namedWindow("cam",1);
		Mat capBuffer, dst;
		int x = 300;
		int y = 300;
		center = Point(x,y);
		for(;;)
		{
			// making start mask from center point	
			cap >> capBuffer;
			frame = capBuffer.clone();
			pickPoint(frame,dst,mask,center,20,20);
			imshow( "Mask", mask );
			DrawCentralPoint( frame, center);
			imshow( "cam", frame);
			if(waitKey(30) >= 0) break;
		}
	}



	//tracking after mask creating (from point after ENTER)
	void HandDetector::Track()
	{
		srand ( time(NULL) );
		VideoCapture cap(0); // open the default camera
		namedWindow("cam",1);
		Mat capBuffer, nextFrame,dst;
		//vectors for optical flow
		vector<Point2f> prevPoints;
		vector<Point2f> nextPoints;
		vector<uchar> statusVector;
		vector<float> errorVector;
		Point2f  nearestPoint;

		for(;;)
		{
			//Finding points to be tracked with optical flow
			FindPointsGFT(frame,prevPoints,mask);
			cap>>capBuffer;
			nextFrame = capBuffer.clone();
			//using and drawing optical flow
			OpticalFlow(frame,nextFrame, prevPoints, nextPoints, statusVector, errorVector);
			DrawOpticalFlow(nextFrame,prevPoints,nextPoints);
			//finding new central point
			nearestPoint = GetNearestPoint(nextPoints);
			//drawing central point (closest to the average)
			DrawCentralPoint( nextFrame, nearestPoint);
			imshow( "Mask", mask );
			imshow( "cam", nextFrame);
			cap >> capBuffer;
			frame = capBuffer.clone();
			//making new mask from central point
			pickPoint(frame,dst,mask,nearestPoint,20,20);

			if(waitKey(30) >= 0) break;
		}
	}
