#include "Filters.h"
#include "Drawing.h"

using namespace cv;
using namespace std;

void DrawOpticalFlow ( Mat& src, vector<Point2f>& prevPoints, vector<Point2f>& nextPoints)
{
	for( int i = 0; i < prevPoints.size(); i++ ) {
		
		line( src, prevPoints[i],nextPoints[i],Scalar(255,0,0),3,CV_AA );
		circle(src, prevPoints[i], 3, Scalar(0,0,255), 3);
	}
}

void DrawHough( Mat &src, vector<Vec4i> &lines)
{
	for( size_t i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];
		line(src, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
	}
}

void DrawHoughCircles ( Mat &dst, vector<Vec3f> circles)
{    
	for( size_t i = 0; i < circles.size(); i++ )
    {
         Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         circle( dst, center, 3, Scalar(0,255,0), -1, 8, 0 );
         circle( dst, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }

}

void DrawCentralPoint ( Mat &dst, Point center)
{
	circle( dst, center , 10, Scalar(0,255,0), 5, 8, 0 );
}

