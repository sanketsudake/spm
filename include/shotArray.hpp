/*!
 * \file shotArray.hpp
 * \brief
 */

#ifndef __SHOTARRAY_H_INCLUDED__
#define __SHOTARRAY_H_INCLUDED__

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

class ShotArray
{
    private:
    	vector<Point> white_positions;
        Point prevPosition;
        time_t shot_start;

    public:
    	ShotArray();
    	~ShotArray();
    	void clearArray();
    	void addPosition(Point position);
    	void drawPath(Mat &frame);
    	double dist(Point p1, Point p2);
    	double totalDist();
    	double totalTime();
    	double shotVelocity();
        double angleError(Point C);
};

#endif
