#include "stdafx.h"
#include "Drawing.h"
#include "MathUtils.h"
#include "ImageUtils.h"


using namespace cv;
using namespace std;

//Conversion from RGB to grayscale image
void ConvertToGray(Mat& src, Mat& src_gray)
{
	cvtColor( src, src_gray, CV_BGR2GRAY );
}


// Finding points to be tracked by optical flow (using GoodFeatures to Track), 
//possible mask parameter, adding random points up to 100
void FindPointsGFT(Mat& src,vector<Point2f>& corners, Mat& mask = Mat())
{
	Mat srcGray, srcEqHist;
	ConvertToGray(src,srcGray);
	equalizeHist(srcGray,srcEqHist);
	/// Parameters for Shi-Tomasi algorithm
	double qualityLevel = 0.01;
	int maxCorners = 100;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.4;

	/// Apply corner detection - TODO: improve
	goodFeaturesToTrack( srcEqHist, corners,	maxCorners,	qualityLevel,
		minDistance, mask, blockSize,	useHarrisDetector, k );


	int n = corners.size();
	printf("Mask size: %d\n", mask.dataend - mask.data);
	vector<Point2f> maskPoints;
	for(int row=0;row<mask.rows;row++) {
		const uchar *ptr = mask.data + row * mask.step;
		for(int col=0;col < mask.cols;col++)
			if (*ptr++ > 0)
				maskPoints.push_back(Point2f(col,row));
	}
	const int dx = 10;
	const int dy = 10;

	for(int i=0; i< 100 - n;i++) {
		int k = rand() % maskPoints.size();	
		corners.push_back(Point2f(maskPoints[k]));
	}


}



// Finding movement among tracked points
void OpticalFlow(Mat& src1, Mat& src2, vector<Point2f>& prevPoints, vector<Point2f>& nextPoints,
				 vector<uchar>& statusVector, vector<float>& errorVector)
{
	Mat srcGray1, srcGray2;
	ConvertToGray(src1,srcGray1);
	ConvertToGray(src2,srcGray2);
	calcOpticalFlowPyrLK(srcGray1,srcGray2, prevPoints, nextPoints, statusVector, errorVector);
}



void pickPoint (Mat &src, MatND &dst, Mat &mask, Point seed, int lo = 20, int up = 20)
{
	  Mat hsv;
	  cvtColor( src, hsv, CV_BGR2HSV );
	  int newMaskVal = 255;
	  Scalar newVal = Scalar( 120, 120, 120 );
	  int connectivity = 8;
	  int flags = connectivity + (newMaskVal << 8 ) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY;
	  Mat mask2 = Mat::zeros( src.rows + 2, src.cols + 2, CV_8UC1 );
	  floodFill( src, mask2, seed, newVal, 0, Scalar( lo, lo, lo ), Scalar( up, up, up), flags );
	 
	  mask = mask2( Range( 1, mask2.rows - 1 ), Range( 1, mask2.cols - 1 ) );

}

////apply FloodFill algorithm for chosen point
//void pickPoint (Mat &src, MatND &dst, Mat &mask, Point seed, int lo = 20, int up = 20)
//{
//	  Mat hsv,mask1, mask3;
//	  int vmin = 100, vmax = 230, smin = 30;
//	 /* Scalar minColor = Scalar(color.val[0]-lo,color.val[1]-lo,color.val[2]-lo);
//	  Scalar maxColor = Scalar(color.val[0]+up,color.val[1]+up,color.val[2]+up);
//	  cvtColor( src, hsv, CV_BGR2HSV );
//	  		inRange(hsv, Scalar(0, smin, vmin),
//							Scalar(180, 256, vmax), mask1);*/
//	  int newMaskVal = 255;
//	  Scalar newVal = Scalar( 120, 120, 120 );
//	  int connectivity = 8;
//	  int flags = connectivity + (newMaskVal << 8 ) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY;
//	  Mat mask2 = Mat::zeros( src.rows + 2, src.cols + 2, CV_8UC1 );
//	  //floodFill( src, mask, seed, newVal, 0, Scalar( lo, lo, lo ), Scalar( up, up, up), flags );	 
//	  floodFill( src, mask2, seed, newVal, 0, Scalar( lo, lo, lo ), Scalar( up, up, up), flags );
//	  // TODO: dalczego czasem rzuca tu wyj¹tek?	 
//	  mask
//		  = mask2( Range( 1, mask2.rows - 1 ), Range( 1, mask2.cols - 1 ) );
//	//  bitwise_and(mask1,mask3,mask);
//	 // imshow("mask3", mask3);
//	  HistAndBackproj( hsv,mask,dst );
//	  //imshow("backpr", dst);
//}

void HistAndBackproj(Mat &hsv, Mat &mask, MatND &dst )
{
  MatND hist;
  int h_bins = 30; int s_bins = 32;
  int histSize[] = { h_bins, s_bins };
  float h_range[] = { 0, 179 };
  float s_range[] = { 0, 255 };
  const float* ranges[] = { h_range, s_range };
  int channels[] = { 0, 1 };
  /// Get the Histogram and normalize it
  calcHist( &hsv, 1, channels, mask, hist, 2, histSize, ranges, true, false );
  normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );
  calcBackProject( &hsv, 1, channels, hist, dst, ranges, 1, true );
}

void ConvertToHSV(Mat &src, Mat &dst)
{
	cvtColor(src, dst, CV_BGR2HSV);
}

void HistNormalize(Mat &hist)
{
normalize(hist, hist, 0, 255, CV_MINMAX);
}