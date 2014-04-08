/*!
 * \file collisionDetector.cpp
 e* \brief Consist methods to process collisions
 */

#include "collisionDetector.hpp"
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


void CollisionDetector::checkCollision(Point position, Mat &previous, Mat &original, ShotArray &shot_array,Point &final, Point &normalEndpoint, int &whiteSize)
{
    Mat diff;
    Mat iGray;
    Mat interest;
    Rect roi;
    int interDistance;
    vector<Point> metaData;
    //! Identifying only 4 collisions
    if(collisionCount > 4)
        return ;

    //! Code need to refactorized
    double currSlope = 1000;

    int xdelta = position.x - prevPoint.x;
    int ydelta = -(position.y - prevPoint.y);

    //! Distance of white ball between 2 frames
    interDistance = distanceCalculate(position, prevPoint);

    if((xdelta || ydelta)){
        if(!xdelta)
            currSlope = 899;
        else
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
                // HoughCircles(iGray, circlesInBox, CV_HOUGH_GRADIENT, 1, iGray.rows/16, 50, 15, 5, 10);
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
                    if(distance>0 and distance<16){
                        temp.push_back(center.x);
                        temp.push_back(center.y);
                        temp.push_back(radius);
                        temp.push_back(distance);
                        circles.push_back(temp);
                    }

                    //Draw detected circles in Bounding-Box
                    // circle(previous, center, 7, Scalar(0, 0 ,0), 2);
                }

                int dots_size = shot_array.white_positions.size();
                if(collisionCount==2 && circles.size()){
                    whiteSize = dots_size;
                    collisionPoint(previous, shot_array.white_positions[0],
                            shot_array.white_positions[dots_size-1],final,normalEndpoint );
                    cout << "White size :" << whiteSize << "  "<< final << " "<< normalEndpoint << endl; 
                }

                //! Wait after every collision
                if(collisionCount < 3){  
                    while(waitKey(1) != 27){
                        // imshow("Previous",previous);
                        // imshow("ROI",interest);
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

int CollisionDetector::perpendicularDistance(Mat &frame, Point startpoint, Point center){

    double slope, ydelta, xdelta;
    double A, B, C;
    double constant;
    int distance;

    xdelta = (startpoint.x - prevPoint.x);
    ydelta = (startpoint.y - prevPoint.y);

    if(!xdelta)
        slope = 899;
    else
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

Point CollisionDetector::generatePoint(int xdelta, int ydelta, extrapolate &line){
    int X, Y;

    if(xDir==1){
        // cout<<"Moving Forward!"<<endl;
        if(abs(xdelta)>=abs(ydelta)){
            X = prevPoint.x+(boxHeight/2);
            Y = line.y(X);
        }
        else{
            if(yDir==1)
                Y = prevPoint.y+(boxHeight/2);
            else
                Y = prevPoint.y-(boxHeight/2);
            X = line.x(Y);
        }
    }
    else if(xDir==-1){
        // cout<<"Moving Backward!"<<endl;
        if(abs(xdelta)>=abs(ydelta)){
            X = prevPoint.x-(boxHeight/2);
            Y = line.y(X);
        }
        else{
            if(yDir==1)
                Y = prevPoint.y+(boxHeight/2);
            else
                Y = prevPoint.y-(boxHeight/2);
            X = line.x(Y);
        }
    }
    else{
        // cout<<"Moving Vertical"<<endl;
        if(yDir==1)
            Y = prevPoint.y+(boxHeight/2);
        else if(yDir==-1) 
            Y = prevPoint.y-(boxHeight/2);
        X = prevPoint.x;
    }

    return Point(X,Y);
}

void CollisionDetector::setDirection(Point startpoint, Point endpoint){
    //! Detect X-Direction
    if(startpoint.x == prevPoint.x)
        xDir = 0;
    else if(startpoint.x < prevPoint.x)
        xDir = 1;
    else if(startpoint.x > prevPoint.x)
        xDir = -1;

    //! Detect Y-Direction
    if(startpoint.y == prevPoint.y)
        yDir = 0;
    else if(startpoint.y < prevPoint.y)
        yDir = 1;
    else if(startpoint.y > prevPoint.y)
        yDir = -1;
}

void CollisionDetector::collisionPoint(Mat &frame, Point startpoint, Point lastpoint, Point &finalPoint, Point &normalEndpoint){

    double slope, ydelta, xdelta;
    double constant;
    int X, Y;
    bool x_direction;
    bool y_direction;

    xdelta = (prevPoint.x - startpoint.x);
    ydelta = (prevPoint.y - startpoint.y);

    slope = (ydelta/xdelta);
    constant = (-(slope*startpoint.x)+startpoint.y);

    // cout<<"Xdelta: "<<xdelta<<endl;
    // cout<<"Ydelta: "<<ydelta<<endl;

    extrapolate expectedLine(slope, startpoint.x, startpoint.y);

    setDirection(startpoint, prevPoint);

    Point expectedPoint = generatePoint(xdelta, ydelta, expectedLine);

    // cout<<"XDir: "<<xDir<<"\t"<<"YDir: "<<yDir<<endl;
    // cout<<"X: "<<expectedPoint.x<<"\tY: "<<expectedPoint.y<<endl;

    //Expected White Line
    line(frame, prevPoint, expectedPoint, Scalar(255, 0, 0), 1, CV_AA);

    // Check for interested circles.
    assert(circles.size()>0);
    int shortest_dist=9000, index=-1;
    // cout<<"Size: "<<circles.size()<<endl;

    for( size_t i = 0; i < circles.size(); i++ ){
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int temp;
        temp=distanceCalculate(center, prevPoint);
        if(temp<shortest_dist){
            index = i;
            shortest_dist=temp;
        }
        // !circles of interest
        // circle(frame, Point(circles[i][0], circles[i][1]), 7, Scalar(90, 255 ,150), 2);
    }

    Point center(cvRound(circles[index][0]), cvRound(circles[index][1]));
    int h = cvRound(circles[index][3]);

    int ab;
    int ac;
    int ad;
    int dc;
    double angle;
    Point final;        //Center of white ball at time of collision
    Point poi;          //Point of interesection

    angle = abs(atan (slope) * 180 / PI);

    ab = distanceCalculate(center, prevPoint);
    ac = round(sqrt(abs(pow(ab,2)-pow(h,2))));
    dc = round(sqrt(abs(pow(14,2)-pow(h,2))));
    ad = abs(ac-dc);

    // cout<<"Angle: "<<angle<<endl;
    // cout<<"Height: "<<h<<endl;
    // cout<<"AB: "<<ab<<endl;
    // cout<<"AC: "<<ac<<endl;
    // cout<<"DC: "<<dc<<endl;
    // cout<<"AD: "<<ad<<endl;

    if(xDir>=0)  
        final.x = prevPoint.x+round(ad*cos(angle*PI/180.0));
    else
        final.x = prevPoint.x-round(ad*cos(angle*PI/180.0));

    if(yDir>=0)  
        final.y = prevPoint.y+round(ad*sin(angle*PI/180.0));
    else
        final.y = prevPoint.y-round(ad*sin(angle*PI/180.0));

    //! White circle at collision
    circle(frame, final, 7, Scalar(0, 0, 0), 2);

    //! Colliding Circle
    circle(frame, Point(circles[index][0], circles[index][1]), 7, Scalar(0, 0 ,0), 2);

    //! Point of intersection
    poi.x = round((final.x+center.x)/2);
    poi.y = round((final.y+center.y)/2);
    circle(frame, poi, 1, Scalar(0, 255, 255), 2);

    double normal_slope;
    double tangent_slope;
    Point normal_endpoint;
    Point tangent_endpoint;
    Point normal_startpoint;
    Point tangent_startpoint;

    xdelta = (final.x - center.x);
    ydelta = (final.y - center.y);

    normal_slope = (ydelta/xdelta);
    tangent_slope = (-1)/(normal_slope);

    extrapolate normalLine(normal_slope, final.x, final.y);
    extrapolate tangentLine(tangent_slope, final.x, final.y);

    normal_endpoint = generatePoint(xdelta, ydelta, normalLine);
    tangent_endpoint = generatePoint(ydelta, xdelta, tangentLine);

    xDir *= -1;
    yDir *= -1;

    normal_startpoint= generatePoint(xdelta, ydelta, normalLine);
    tangent_startpoint= generatePoint(ydelta, xdelta, tangentLine);
    //normal
    line(frame, normal_startpoint, normal_endpoint, Scalar(18, 158, 230), 1, CV_AA);

    //tangent
    line(frame, tangent_startpoint, tangent_endpoint, Scalar(255, 255, 0), 1, CV_AA);

    finalPoint = final;
    normalEndpoint = normal_endpoint;
}

