/*!
 * \file shot.cpp
 * \shot related parameters, suggested shots.
 */
#include "shot.hpp"

using namespace std;
using namespace cv;


void onMouseClick(int event, int x, int y, int flags, void* userdata)
{
    Point *p = ((Point *)userdata);
    if(event == EVENT_LBUTTONDOWN)
    {
        //cout << "Mouseclick " << x << " " << y << endl;
        if((x>=68+7 && x<=1091-7) && (y>=73+7 && y<=603-7))
        {
            *p = Point(x, y);
        }
    }
}


Shot::Shot()
{
    startpoint = Point(-1, -1);
    p1 = Point(1000, 10000);
    p2 = Point(1000, 10000);
    angleError = 0;
}

Shot::~Shot()
{

}

void Shot::getUserInput(Mat &frame)
{
    if(p1.x != -1)
        return;
    setMouseCallback("Snooker Player Profile Management", onMouseClick, &p1);
    while(p1.x == -1 && p1.y==-1){
        putText(frame, "Suggest Point", Point(startpoint.x, startpoint.y - 10), 1, 1, Scalar(150, 255, 255), 2);
        imshow("Snooker Player Profile Management", frame);
        waitKey(5);

        // if(p1.x != -1)
        // 	cout << "Got value " << p1.x << " " << p1.y << endl;
    }
    drawSuggested(frame);
    setMouseCallback("Snooker Player Profile Management", onMouseClick, &p2);
    while(p2.x == -1 && p2.y==-1){
        putText(frame, "Next point", Point(p1.x -10, p1.y-10), 1, 1, Scalar(150, 255, 255), 2);
        imshow("Snooker Player Profile Management", frame);
        waitKey(5);
        // if(p2.x != -1)
        // 	cout << "Got value " << p2.x << " " << p2.y << endl;
    }

    setMouseCallback("Snooker Player Profile Management", NULL, NULL);
}

void Shot::clear()
{
    p1 = Point(-1, -1);
    p2 = Point(-1, -1);
}

void Shot::setShotStartP(Point position)
{
    startpoint = position;
}

void Shot::drawSuggested(Mat &frame)
{
    if(startpoint.x != -1 && p1.x != -1)
        line(frame, startpoint, p1, Scalar(0, 100, 0), 2, CV_AA);
    if(p1.x != -1 && p2.x != -1)
        line(frame, p1, p2, Scalar(0, 100, 0), 2, CV_AA);
    if(p1.x != -1)
        circle(frame, p1, 2, Scalar(255, 255  ,255), 2);
    if(p2.x != -1)
        circle(frame, p2, 2, Scalar(255, 255  ,255), 2);

}

double Shot::showFeedback(Mat &frame, ShotArray *shot_array,string shottype)
{
    //using cosine law
    angleError = abs(shot_array->angleError(p1) * 180 / M_PI);
    stringstream ss;
    ss << "AngleError : " << angleError << " deg";
    cout << "\t\"angle_error\" : " << angleError << endl;
    putText(frame, ss.str(),
            Point(600, 600), 1, 1, Scalar(255, 255, 255), 2);
    ss.str("Shot Type : ");
    //   ss<<shottype;
    putText(frame, ss.str()+shottype,
            Point(600, 550), 1, 1, Scalar(255, 255, 255), 2);

    return angleError;
}


void Shot::velocityError()
{

}

void Shot::rateShot()
{

}
/*
 *TODO: Need to complete this as soon as possible
 */
int Shot::shotType()
{
    //1: straight 2: cut 3: spin
    cout << "shot Type: Straight" << endl;
    return 1;

}
void Shot :: shotType(Point final, Point normalEndpoint, Point white){
    double b = (double)sqrt(pow((white.x - normalEndpoint.x), 2) + pow((white.y - normalEndpoint.y), 2));

    double c = (double)sqrt(pow((final.x - normalEndpoint.x), 2) + pow((final.y - normalEndpoint.y), 2));
    double a = (double)sqrt(pow((final.x - white.x), 2) + pow((final.y - white.y), 2));
    //angle between normal and final position of white ball.
    //Calculated for purpose of spin detection.
    double angle = acos(((a*a) + (c*c) - (b*b))/(2*a*c))*180/3.1415926;

}
double Shot :: getSuggDist(){
    return (double)sqrt(pow((startpoint.x - p1.x), 2) + pow((startpoint.y - p1.y), 2));
}
