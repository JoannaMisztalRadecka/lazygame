#include "stdafx.h"
#include "Drawing.h"
#include "MathUtils.h"
#include "ImageUtils.h"

using namespace cv;
using namespace std;

class HandDetector
{
//private:
	Point center;
	Mat mask, frame;
	Scalar color;
public:
	void Calibration();
	void Track();
	void CamShiftTracking();
	
};