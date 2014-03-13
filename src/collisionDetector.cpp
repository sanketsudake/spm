/*!
 * \file collisionDetector.cpp
 * \brief Consist methods to process collisions
 */

#include "collisionDetector.hpp"
#include <math.h>
#include <vector>

using namespace std;
using namespace cv;


int CollisionDetector::distanceCalculate(Point p1, Point p2)
{
    int x = p1.x - p2.x;
    int y = p1.y - p2.y;
    double dist;

    dist = sqrt(pow(double(x),2)+pow(double(y),2));           

    return int(dist);
}


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

    slopeTheta = 0.16;
    boxWidth = 200;
    boxHeight = 200;

    // circlesInBox.clear();
    // circles.clear();
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
            // cout<<"SlopeTheta: "<<slopeTheta<<endl;
            // cout<<"Diff: "<<abs(prevSlope - currSlope)<<endl;
            if(abs(prevSlope - currSlope) > slopeTheta)
            {
                //! calculate the difference
                // in previous and current frame.

                collPoints[collisionCount] = prevPoint;
                collisionCount++;
                // slopeTheta += 0.018;

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

                roi = Rect(box_x, box_y, boxWidth, boxHeight);
                interest = previous(roi);

                //use houghcircles to detect balls in roi
                cvtColor(interest, iGray, CV_BGR2GRAY);
                HoughCircles(iGray, circlesInBox, CV_HOUGH_GRADIENT, 1, iGray.rows/16, 50, 15, 5, 10);

                for( size_t i = 0; i < circlesInBox.size(); i++ ) {

                    circlesInBox[i][0] += box_x;
                    circlesInBox[i][1] += box_y;
                    vector<int> temp;

                    Point center(cvRound(circlesInBox[i][0]), cvRound(circlesInBox[i][1]));
                    int radius = cvRound(circlesInBox[i][2]);
                    int distance;

                    distance = perpendicularDistance(previous, shot_array.white_positions[0], center);
                    // cout<<"distance: "<<distance<<endl;
                    if(distance>0 and distance<18){
                        temp.push_back(center.x);
                        temp.push_back(center.y);
                        temp.push_back(radius);
                        temp.push_back(distance);
                        circles.push_back(temp);
                    }

                    //Draw detected circles in Bounding-Box
                    // circle(previous, center, 7, Scalar(0, 0 ,0), 2);
                }

                if(collisionCount==2)
                    collisionPoint(previous, shot_array.white_positions[0]);

                //! Wait after every collision
                while(waitKey(1) != 27){
                    imshow("Previous",previous);
                    imshow("ROI",interest);
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

int CollisionDetector::perpendicularDistance(Mat &frame, Point startpoint, Point center){

    double slope, ydelta, xdelta;
    double A, B, C;
    double constant;
    int distance;

    xdelta = (startpoint.x - prevPoint.x);
    ydelta = (startpoint.y - prevPoint.y);

    slope = (ydelta/xdelta);
    constant = (-(slope*startpoint.x)+startpoint.y);

    // cout<<"Slope: "<<slope<<endl;
    // cout<<"Starpoint: "<<startpoint.x<<"\t"<<startpoint.y<<endl;
    // cout<<"Endpoint: "<<prevPoint.x<<"\t"<<prevPoint.y<<endl;
    // cout<<"A: "<<A<<"\tB: "<<B<<"\tC: "<<C<<endl;

    A = -(slope);
    B = 1.0;
    C = -constant;

    distance = int(abs(A*(center.x)+B*(center.y)+C)/(sqrt((A*A)+(B*B))));
    return distance;
}

void CollisionDetector::collisionPoint(Mat &frame, Point startpoint){
    cout<<"Collision Point Detection!"<<endl;

    double slope, ydelta, xdelta;
    double constant;
    int X, Y;
    bool x_direction;
    bool y_direction;

    xdelta = (startpoint.x - prevPoint.x);
    ydelta = (startpoint.y - prevPoint.y);

    slope = (ydelta/xdelta);
    constant = (-(slope*startpoint.x)+startpoint.y);
    
    extrapolate expectedLine(slope, startpoint.x, startpoint.y);
     
    if(xdelta){
        // Ball Not moving along Y-axis
        if(startpoint.x < prevPoint.x){
            cout<<"Moving Forward!"<<endl;
            if(ydelta>=xdelta){
                X = prevPoint.x+(boxWidth/2);
                Y = expectedLine.y(X);
            }
            else{
                Y = prevPoint.y-(boxHeight/2);
                X = expectedLine.x(Y);
            }
            x_direction = 1;
        }
        else{
            cout<<"Moving Backward!"<<endl;
            if(ydelta>=xdelta){
                X = prevPoint.x-(boxWidth/2);
                Y = expectedLine.y(X);
            }
            else{
                Y = prevPoint.y+(boxHeight/2);
                X = expectedLine.x(Y);
            }
            x_direction = 0;
        }
    }

    else{
        // Ball moving along Y-axis
        if(startpoint.y < prevPoint.y){
            cout<<"Moving Downward!"<<endl;
            Y = prevPoint.y+(boxHeight/2); 
            X = prevPoint.x;
            y_direction = 1;
        }
        else{
            cout<<"Moving Upward!"<<endl;
            Y = prevPoint.y-(boxHeight/2); 
            X = prevPoint.x;
            y_direction = 0;
        }
    }

    line(frame, prevPoint, Point(X,Y), Scalar(255, 0, 0), 2, CV_AA);

    int shortest_dist=9000, index=-1;

    assert(circles.size()>0);

    cout<<"Size: "<<circles.size()<<endl;
    for( size_t i = 0; i < circles.size(); i++ ){
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int temp;
        temp=distanceCalculate(center, prevPoint);
        if(temp<shortest_dist){
            index = i;
            shortest_dist=temp;
        }
        // !circles of interest
        // circle(frame, Point(circles[i][0], circles[i][1]), 7, Scalar(0, 0 ,0), 2);
    }

    // circle(frame, Point(circles[index][0], circles[index][1]), 7, Scalar(0, 0 ,0), 2);

    Point center(cvRound(circles[index][0]), cvRound(circles[index][1]));
    int h = cvRound(circles[index][3]);
    
    int ab;
    int ac;
    int ad;
    int dc;
    double angle;
    Point final;        //Center of white ball at time of collision

    angle = atan (slope) * 180 / PI;

    ab = distanceCalculate(center, prevPoint);
    ac = int(sqrt(abs(pow(ab,2)-pow(h,2))));
    dc = int(sqrt(abs(pow(14,2)-pow(h,2))));
    ad = abs(ac-dc);

    cout<<"Angle: "<<angle<<endl;
    cout<<"Height: "<<h<<endl;
    cout<<"AB: "<<ab<<endl;
    cout<<"AC: "<<ac<<endl;
    cout<<"DC: "<<dc<<endl;
    cout<<"AD: "<<ad<<endl;

    if(x_direction)  
        final.x = prevPoint.x+(ad*cos(angle*PI/180.0));
    else
        final.x = prevPoint.x-(ad*cos(angle*PI/180.0));

    if(y_direction)  
        final.y = prevPoint.y+(ad*sin(angle*PI/180.0));
    else
        final.y = prevPoint.y-(ad*sin(angle*PI/180.0));

    circle(frame, final, 7, Scalar(25, 25, 25), 2);

    double normal_slope;
    double tangent_slope;
    xdelta = (final.x - center.x);
    ydelta = (final.y - center.y);
    
    normal_slope = (ydelta/xdelta);
    tangent_slope = (-1)/(normal_slope);
    
    extrapolate normalLine(normal_slope, final.x, final.y);
    extrapolate tangentLine(tangent_slope, final.x, final.y);

    line(frame, final, Point(final.x-90, normalLine.y(final.x-90)), Scalar(173, 55, 173), 2, CV_AA);
    line(frame, final, Point(final.x+90, tangentLine.y(final.x+90)), Scalar(255, 255, 0), 2, CV_AA);
}
