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

using namespace std;
using namespace cv;

/*!
 * \class DetectShot
 * \brief
 */
class DetectShot
{
private:
	Mat currentFrame, back, fore;
    BackgroundSubtractorMOG2 bg;
	vector< vector<Point> > contours;

public:
	int shotcount;
    DetectShot();
	~DetectShot();
	int BgSubtractor(Mat &frame);
};

#endif
