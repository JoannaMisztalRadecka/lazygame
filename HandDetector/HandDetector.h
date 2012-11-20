#include "Drawing.h"
#include "MathUtils.h"
#include "ImageUtils.h"
#include <stdio.h>
#include <stdlib.h>


using namespace cv;
using namespace std;

class HandDetector
{
//private:
	Point center;
	Mat mask, frame;
public:
	void Calibration();
	void Track();
};