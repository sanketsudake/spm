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


void CollisionDetector::checkCollision(Point position, Mat &previous, Mat &original, ShotArray &shot_array)
{
    Mat diff;
    Mat iGray;
    Mat interest;
    Rect roi;

    int box_x, box_y;

    int boxWidth = 200;
    int boxHeight = 200;


    //! Identifying only 4 collisions
    if(collisionCount > 4)
        return;

    //! Code need to refactorized
    double currSlope = 1000;

    int xdelta = position.x - prevPoint.x;
    int ydelta = -(position.y - prevPoint.y);

    if(xdelta){
        currSlope = ((double)(ydelta)/(xdelta));

        if(prevSlope < 900)
        {
            if(abs(prevSlope - currSlope) > slopeTheta)
            {
                if(!(abs(xdelta) <= 2 && abs(ydelta) <= 2))
                {
                    //! calculate the difference
                    // in previous and current frame.

                    collPoints[collisionCount] = prevPoint;
                    collisionCount++;
                    slopeTheta += 0.018;

                    /*
                     *  Create Bounding Box. 
                     */

                    if(prevPoint.x-100>0)
                        box_x = prevPoint.x-100;
                    else
                        box_x = 0;

                    if(prevPoint.y-100>0)
                        box_y = prevPoint.y-100;
                    else
                        box_y = 0;

                    if(prevPoint.x+100<TWIDTH)
                        boxWidth = 200;
                    else
                    {
                        boxWidth = (TWIDTH-prevPoint.x)*2;
                        box_x = prevPoint.x - boxWidth/2;
                    }

                    if(prevPoint.y+100<THEIGHT)
                        boxHeight = 200;
                    else
                    {
                        boxHeight = (THEIGHT-prevPoint.y)*2; 
                        box_y = prevPoint.y - boxHeight/2;
                    }

                    // cout<<"X: "<<x<<"\tY: "<<y<<endl;
                    roi = Rect(box_x, box_y, boxWidth, boxHeight);
                    interest = previous(roi);

                    //use houghcircles to detect balls in roi
                    cvtColor(interest, iGray, CV_BGR2GRAY);
                    HoughCircles(iGray, circlesInBox, CV_HOUGH_GRADIENT, 1, iGray.rows/16, 50, 15, 5, 10);

                    //Draw detected circles in Bounding-Box
                    for( size_t i = 0; i < circlesInBox.size(); i++ ) {

                        circlesInBox[i][0] += box_x;
                        circlesInBox[i][1] += box_y;

                        Point center(cvRound(circlesInBox[i][0]), cvRound(circlesInBox[i][1]));
                        int radius = cvRound(circlesInBox[i][2]);

                        // circle( interest, center, 3, Scalar(150,255,150), -1, 8, 0 );
                        circle( previous, center, 3, Scalar(150,255,150), -1, 8, 0 );
                        // circle( interest, center, 3, Scalar(150,255,150), -1, 8, 0 );
                        circle( previous, center, radius, Scalar(0,0,0), 2, 8, 0 );    
                        
                        // line(previous, prevPoint, center, Scalar(0, 0, 0), 1, CV_AA);
                    }

                    if(collisionCount==2)
                        expectedWhiteLine(previous, box_x, box_y, shot_array.white_positions[0]);
                        // expectedWhiteLine(previous, boxWidth, boxHeight, shot_array.white_positions[0]);

                    //! Wait after every collision
                    while(waitKey(1) != 27){
                        imshow("Previous", previous);
                        imshow("ROI", interest);
                    }
                }
            }
        }
    }

    prevPoint = position;
    prevSlope = currSlope;
}


void CollisionDetector::shotType()
{
    // We have first four apporox. collision type
    // Need to shot type from that info
}

void CollisionDetector::expectedWhiteLine(Mat &frame, int X, int Y, Point startpoint){

    // cout<<"\nIn expectedWhiteLine"<<endl; 
    
    Point interesection;

    double slope, ydelta, xdelta;
    double A, B, C;
    double constant;
    double distance;
    double x, y; 

    xdelta = (startpoint.x - prevPoint.x);
    ydelta = (startpoint.y - prevPoint.y);

    slope = (ydelta/xdelta);
    constant = (-(slope*startpoint.x)+startpoint.y);

    extrapolate expectedLine(slope, startpoint.x, startpoint.y);

    A = -(slope);
    B = 1.0;
    C = -constant;

    cout<<"Slope: "<<slope<<endl;
    cout<<"Starpoint: "<<startpoint.x<<"\t"<<startpoint.y<<endl;
    cout<<"Endpoint: "<<prevPoint.x<<"\t"<<prevPoint.y<<endl;
    cout<<"A: "<<A<<"\tB: "<<B<<"\tC: "<<C<<endl;

    if(xdelta){
        // Ball Not moving along Y-axis
        if(startpoint.x < prevPoint.x){
            // cout<<"Moving Forward!"<<endl;
            if(ydelta>=xdelta){
                x = prevPoint.x+60;
                y = expectedLine.y(x);
            }
            else{
                y = prevPoint.y+60;
                x = expectedLine.x(y);
            }
        }
        else{
            // cout<<"Moving Backward!"<<endl;
            x = prevPoint.x-60;
            y = expectedLine.y(x);
        }
    }

    else{
        // Ball moving along Y-axis
        if(startpoint.y < prevPoint.y){
            // cout<<"Moving Downward!"<<endl;
        }
        else{
            // cout<<"Moving Upward!"<<endl;
        }
    }

    // line(frame, Point(prevPoint.x-x,prevPoint.y-y), startpoint, Scalar(0, 0, 0), 1, CV_AA);
    line(frame, prevPoint, Point(x,y), Scalar(255, 0, 0), 1, CV_AA);
    cout<<"X: "<<x<<"Y: "<<y<<endl;
   
    for( size_t i = 0; i < circlesInBox.size(); i++ ){

        Point center(cvRound(circlesInBox[i][0]), cvRound(circlesInBox[i][1]));
        int radius = cvRound(circlesInBox[i][2]);
        distance = 0.0;        

        cout<<"Center: "<<center.x<<"\t"<<center.y<<endl;
        cout<<"Radius: "<<radius<<endl;

        distance = abs(A*(center.x)+B*(center.y)+C)/(sqrt((A*A)+(B*B)));
        line(frame, center, prevPoint, Scalar(255, 255, 255), 1, CV_AA);
        // line(frame, prevPoint, Point(x,y), Scalar(0, 0, 0), 1, CV_AA);
        cout<<"Distance: "<<distance<<endl;
    }
}
