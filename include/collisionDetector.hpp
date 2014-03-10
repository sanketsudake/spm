#ifndef __COLLISIONDETECTOR_H_INCLUDED__
#define __COLLISIONDETECTOR_H_INCLUDED__

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include "shotArray.hpp"
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
        vector<Vec3f> circlesInBox; 
        // extrapolate expectedLine;

    public:
        struct extrapolate{
            public:
                double _slope, _x1, _y1;

                extrapolate(double slope, double x1, double y1)
                    : _slope(slope), _x1(x1), _y1(y1)
                {
                }

                double y(double x) const // return y for given x
                {
                    return _y1 + (x-_x1)*_slope;
                }

                double x(double y) const // return x for given y
                {
                    // return _y1 + (x-_x1)*_slope;
                    return (y-_y1+_x1) / _slope;
                }
        };

        CollisionDetector();
        ~CollisionDetector();
        void reset();
        void setShotStartPoint(Point position);
        void drawPrev(Mat &frame);
        void checkCollision(Point position, Mat &frame, Mat &original, ShotArray &);
        void shotType();
        void expectedWhiteLine(Mat&, int, int, Point);
};

#endif
