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

//Camshift tracking from central point - works better with avoiding background but worse with mixing face
	void HandDetector::CamShiftTracking()
	{
		Mat image;
		Mat frame, hsv, hue, mask, mask1, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
		bool selectObject = false;
		int trackObject = 0;
		Point center;
		Rect selection;
		int vmin = 100, vmax = 230, smin = 30;
		center = Point(300,300); 
		selection.x = center.x-50;
		selection.y = center.y-50;
		selection.width = 100;
		selection.height = 100;
		trackObject = -1;
		VideoCapture cap;
		Rect trackWindow;
		int hsize = 16;
		float hranges[] = {0,180};
		const float* phranges = hranges;

		cap.open(0);
		namedWindow( "CamShift Demo", 0 );

		
		for(;;)
		{		
			cap >> frame;
			frame.copyTo(image);
			if(trackObject<0)circle( image, center , 10, Scalar(0,255,0), 5, 8, 0 );
			imshow( "CamShift Demo", image );
			if(waitKey(30) >= 0) 
				break;
		}
		
		for(;;)
		{
			cap >> frame;
			frame.copyTo(image);
			ConvertToHSV(image,hsv);
			if( trackObject )
				{			
					inRange(hsv, Scalar(0, smin, vmin),	Scalar(180, 256,vmax), mask1);
					int ch[] = {0, 0};
					hue.create(hsv.size(), hsv.depth());
					mixChannels(&hsv, 1, &hue, 1, ch, 1);

					if( trackObject < 0 )
					{
						Mat roi(hue, selection), maskroi(mask1, selection);
						calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
						HistNormalize(hist);
						trackWindow = selection;
						trackObject = 1;
						
						histimg = Scalar::all(0);
						int binW = histimg.cols / hsize;
						Mat buf(1, hsize, CV_8UC3);
						for( int i = 0; i < hsize; i++ )
							buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
						cvtColor(buf, buf, CV_HSV2BGR);

						for( int i = 0; i < hsize; i++ )
						{
							int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
							rectangle( histimg, Point(i*binW,histimg.rows),
									   Point((i+1)*binW,histimg.rows - val),
									   Scalar(buf.at<Vec3b>(i)), -1, 8 );
						}
					}

					calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
					backproj &= mask1;
					RotatedRect trackBox = CamShift(backproj, trackWindow,
										TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
				    center = trackBox.center;
					if( trackWindow.area() <= 1 )
					{
						int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
						trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
										   trackWindow.x + r, trackWindow.y + r) &
									  Rect(0, 0, cols, rows);
					}

					ellipse( image, trackBox, Scalar(0,0,255), 3, CV_AA );
				}
	        
			if( selectObject && selection.width > 0 && selection.height > 0 )
			{
				Mat roi(image, selection);
				bitwise_not(roi, roi);
			}

			imshow( "CamShift Demo", image );
			
			imshow( "mask1", mask1 );
			if(waitKey(30)>=0) break;	        
		}
	}

