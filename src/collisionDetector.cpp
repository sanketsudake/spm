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


void CollisionDetector::checkCollision(Point position, Mat &previous, Mat &original)
{
    Mat diff;
    Mat interest;
    Rect roi;

    int x, y;

    int boxWidth = 200;
    int boxHeight = 200;

    // vector< vector<Point> > contours;
    // vector<Vec4i>  hierarchy;

    //! Identifying only 4 collisions
    if(collisionCount > 6)
        return;

    //! Code need to refactorized
    double currSlope = 1000;

    //! Temporary code need to removed
    // cout << "W.Position " << position.x
    // 	 << " "
    // 	 << position.y << " ";

    int xdelta = position.x - prevPoint.x;
    int ydelta = position.y - prevPoint.y;

    if(xdelta){
        currSlope = ((double)(ydelta)/(xdelta));

        // if(ydelta==0){
        //     cout<<"Ball moving horizontally!"<<endl;

        //     // Ball moving forward
        //     // if(xdelta>0) 
        //         // roi = Rect(prevPoint.x-10, prevPoint.y-10, 100, 100);
        //         // Ball moving backward 
        //     // else
        //         // roi = Rect(prevPoint.x-10, prevPoint.y-10, 100, 100);
        // }

        // else if(xdelta==0){
        //     cout<<"Ball moving vertically!"<<endl;
        //     roi = Rect(prevPoint.x-10, prevPoint.y-10, 100, 100);
        // }

        // else{
        //     // First or Third Quadrant
        //     if(currSlope>0){
        //         if(xdelta>0)
        //             cout<<"Ball moving in Quadrant IV"<<endl;
        //         else
        //             cout<<"Ball moving in Quadrant II"<<endl;
        //     }

        //     // Second or Fourth Quadrant
        //     else{
        //         if(xdelta>0)
        //             cout<<"Ball moving in Quadrant I"<<endl;
        //         else
        //             cout<<"Ball moving in Quadrant III"<<endl;
        //     }
        // }

        // cout <<endl<<"curslope , prevSlop"<< currSlope <<  " ";
        // cout << prevSlope - currSlope ;
        // Redundant code => if statement can merged

        // cout<<"Current Slope: "<<currSlope<<endl;
        if(prevSlope < 900)
        {
            if(abs(prevSlope - currSlope) > slopeTheta)
            {
                if(!(abs(xdelta) <= 2 && abs(ydelta) <= 2))
                {
                    //! calculate the difference
                    // in previous and current frame.

                    diff = abs(original - previous);
                    cvtColor(diff, diff, CV_RGB2GRAY);
                    // findContours(diff, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
                    // cout<<"Size:"<<contours.size();

                    // threshold(diff, diff, 30, 255, 0);

                    //cout << "Collision" ;
                    collPoints[collisionCount] = prevPoint;
                    collisionCount++;
                    slopeTheta += 0.018;

                    /*
                     *  Create Bounding Box. 
                     */

                    cout<<"Position: "<<prevPoint.x<<"\t"<<prevPoint.y<<endl;

                    if(prevPoint.x-100>0)
                        x = prevPoint.x-100;
                    else
                        x = 0;

                    if(prevPoint.y-100>0)
                        y = prevPoint.y-100;
                    else
                        y = 0;

                    if(prevPoint.x+100<TWIDTH)
                        boxWidth = 200;
                    else
                    {
                        boxWidth = (TWIDTH-prevPoint.x)*2;
                        x = prevPoint.x - boxWidth/2;
                    }

                    if(prevPoint.y+100<THEIGHT)
                        boxHeight = 200;
                    else{
                        boxHeight = (THEIGHT-prevPoint.y)*2; 
                        y = prevPoint.y - boxHeight/2;
                    }

                    roi = Rect(x, y, boxWidth, boxHeight);
                    interest = previous(roi);
                    //use houghcircles to detect balls in roi
                    Mat iGray;
                    cvtColor(interest, iGray, CV_BGR2GRAY);
                    vector<Vec3f> circles;

                    HoughCircles(iGray,circles, CV_HOUGH_GRADIENT,1,iGray.rows/16,50,15,5,10);
                    cout << "\nrows :"<<iGray.rows;
                    for( size_t i = 0; i < circles.size(); i++ ) {
                        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                        int radius = cvRound(circles[i][2]);

                        /* circle center*/
                        // circle( interest, center, 3, Scalar(150,255,150), -1, 8, 0 );

                        /* circle outline*/
                        circle( interest, center, radius, Scalar(0,0,0), 3, 8, 0 );    
                    }


                    //! Wait after every collision
                    while(waitKey(1) != 27){
                        imshow("Previous", interest);
                        // imshow("Previous", previous);
                        // imshow("Difference", diff);
                    }
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
