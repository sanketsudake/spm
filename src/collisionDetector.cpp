/*!
 * \file collisionDetector.cpp
 * \brief Consist methods to process collisions
 */

#include "collisionDetector.hpp"
#include <math.h>
#include <vector>
using namespace std;
using namespace cv;


CollisionDetector:: CollisionDetector()
{
    reset();
}

CollisionDetector:: ~CollisionDetector()
{
}

void CollisionDetector:: reset()
{
    prevPoint = Point(-1, -1);
    prevSlope = 1000;
    collisionCount = 1;
    for(int i = 0; i < 10; i++)
        collPoints[i] = Point(-1, -1);
    slopeTheta = 0.19;
}

void CollisionDetector::setShotStartPoint(Point position)
{
    collPoints[0] = position;
}

void CollisionDetector::drawPrev(Mat &frame)
{
    for(int i = 1; i < collisionCount; i++)
        circle(frame, collPoints[i], 10, Scalar(255, 0 ,0), 2);
}



void CollisionDetector::checkCollision(Point position)
{
    //! Identifying only 4 collisions
    if(collisionCount > 4)
        return;

    //! Code need to refactorized
    double currSlope = 1000;

    //! Temporary code need to removed
    // cout << "W.Position " << position.x
    // 	 << " "
    // 	 << position.y << " ";
    // cout << prevPoint.y - position.y << " "
    // 	 << prevPoint.x - position.x << " ";

    int xdelta = position.x - prevPoint.x;
    int ydelta = position.y - prevPoint.y;

    if(xdelta)
    {
        currSlope = ((double)(ydelta)/(xdelta));
        // cout <<endl<<"curslope , prevSlop"<< currSlope <<  " ";
        // cout << prevSlope - currSlope ;
        // Redundant code => if statement can merged
        if(prevSlope < 900)
        {
            if(abs(prevSlope - currSlope) > slopeTheta)
            {
                if(!(abs(xdelta) <= 2 && abs(ydelta) <= 2))
                {

                    //cout << "Collision" ;
                    collPoints[collisionCount] = prevPoint;
                    collisionCount++;
                    slopeTheta += 0.018;
                    //! Wait after every collision
                    //while(waitKey(1) != 27);
                }
            }
        }
    }
    //cout << endl;
    prevPoint = position;
    prevSlope = currSlope;
}



void CollisionDetector::shotType()
{
    // We have first four apporox. collision type
    // Need to shot type from that info
    //

}
