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

//! Blank frames between two shots
//! To be adjusted according to video
#define SHOT_TEMP_COUNT 20

using namespace std;
using namespace cv;

class ShotArray
{
private:
	vector<Point> white_positions;
public:
	ShotArray();
	~ShotArray();
	void clearArray();
	void addPosition(Point position);
	void drawPath(Mat &frame);
};


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
	int shot_state, shottemp;
public:
    //! Maintains global shot count
	int shotcount;
    DetectShot();
	~DetectShot();
	int BgSubtractor(Mat &frame);
	void shotChecker(Mat &frame, ShotArray *shotarray);
	void shotTrigger(Mat &frame, ShotArray *shotarray);
	void preshotTrigger(Mat &frame);
	void displayShotnumber(Mat &frame);
};

#endif
