#ifndef __COLLISIONDETECTOR_H_INCLUDED__
#define __COLLISIONDETECTOR_H_INCLUDED__

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <iostream>
#include <cmath>

#define TWIDTH 1140 
#define THEIGHT 652 

using namespace std;
using namespace cv;

class CollisionDetector
{
    private:
        Point prevPoint, collPoints[10];
        double prevSlope, slopeTheta;
        int  collisionCount;

    public:
        CollisionDetector();
        ~CollisionDetector();
        void reset();
        void setShotStartPoint(Point position);
        void drawPrev(Mat &frame);
        void checkCollision(Point position, Mat &frame, Mat &original);
        void shotType();
};

#endif
