/*!
 * \file detectshot.cpp
 * \brief Consist methods to process shot
 */

#include "detectshot.hpp"

using namespace std;
using namespace cv;

DetectShot::DetectShot()
{
	shotcount = 0;
}

DetectShot::~DetectShot()
{

}

int DetectShot::BgSubtractor(Mat &frame)
{
	bg.operator ()(frame, fore);
	bg.getBackgroundImage(back);
	erode(fore, fore, Mat());
	dilate(fore, fore, Mat());
	findContours(fore, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//drawContours(frame,  contours,  -1,  Scalar(0,  0,  255),  2);
	if(contours.size() == 0)
	{
		shotcount++;
		//! Show shot end
		stringstream ss;
		ss << "Shot End : " << shotcount ;
		putText(frame, ss.str() , Point(600, 500), 1, 1, Scalar(200, 200, 200), 2);
	}
	return contours.size();
}
