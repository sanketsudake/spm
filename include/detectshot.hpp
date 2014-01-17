/*!
 * \file detectshot.hpp
 * \brief
 */

#ifndef __DETECTSHOT_H_INCLUDED__
#define __DETECTSHOT_H_INCLUDED__
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <iostream>
#include <cmath>
//! Blank frames between two shots
//! To be adjusted according to video
#define SHOT_TEMP_COUNT 20

using namespace std;
using namespace cv;



/*!
 * \class DetectShot
 * \brief
 */
class DetectShot
{
private:
    //! Matrix required for background sub.
	Mat currentFrame, back, fore;
    //! Instance of background subtraction
    BackgroundSubtractorMOG2 bg;
    //! Vector to store contours
	vector< vector<Point> > contours;
    //! Previous postion of white ball
	Point prevPosition;
    //! Temporary variables in shot detection
	int shot_state, shottemp, shotstart, flag, contour_temp;
public:
    //! Maintains global shot count
	int shotcount;
    DetectShot();
	~DetectShot();
	int BgSubtractor(Mat &frame);
	int shotChecker(Mat &frame, Point position);
	void shotTrigger(Mat &frame);
	void preshotTrigger(Mat &frame);
	void displayShotnumber(Mat &frame);
	int isWhiteMoving(Point position);
};

#endif
