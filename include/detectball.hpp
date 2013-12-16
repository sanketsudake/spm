/*!
 * \file detectball.hpp
 * \brief
 */

#ifndef __DETECTBALL_H_INCLUDED__
#define __DETECTBALL_H_INCLUDED__
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
using namespace cv;

/*!
 * \class DetectBall
 * \brief
 */
class DetectBall
{
private:
	//! max number of objects to be detected in frame
	static const int MAX_NUM_OBJECTS = 50;
	//! minimum and maximum object area
	static const int MIN_OBJECT_AREA = 11 * 11;
	static const int MAX_OBJECT_AREA = 20 * 20;
	Scalar *white_minval, *white_maxval;
public:
	DetectBall();
	~DetectBall();
	void morphOps(Mat &frame);
	Point trackFilteredObject(Mat &frame);
	Point detectWhite(Mat &frame);
	void mapPosition(Mat &frame, Point position);
};

#endif
