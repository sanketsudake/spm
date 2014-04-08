#ifndef __COLLISIONDETECTOR_H_INCLUDED__
#define __COLLISIONDETECTOR_H_INCLUDED__

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include "shotArray.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

#define TWIDTH 1140 
#define THEIGHT 652 
#define PI 3.14159265

using namespace std;
using namespace cv;

class CollisionDetector
{
    private:
        Point prevPoint, collPoints[10];
        double prevSlope, slopeTheta;
        int  collisionCount;
        int box_x, box_y;
        int xDir;
        int yDir;
        int boxWidth;
        int boxHeight;

        vector<Vec3f> circlesInBox; 
        vector< vector<int> > circles;

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
                    return ((y-_y1)+(_x1 * _slope)) / _slope;
                }
        };

        CollisionDetector();
        ~CollisionDetector();
        void reset();
        void setShotStartPoint(Point position);
        void drawPrev(Mat &frame);
        void shotType();
        void checkCollision(Point position, Mat &frame, Mat &original, ShotArray &,Point &, Point &, int &);
        void collisionPoint(Mat &, Point , Point, Point &, Point &);
        int perpendicularDistance(Mat&, Point, Point);
        int distanceCalculate(Point, Point);
        Point generatePoint(int , int , extrapolate &);
        void setDirection(Point, Point);
};

#endif
