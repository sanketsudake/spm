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
        putText(frame, "Specify Point", Point(750, 40), 1, 1, Scalar(255, 0, 0), 2);
        imshow("Snooker Player Profile Management", frame);
        waitKey(5);

        // if(p1.x != -1)
        // 	cout << "Got value " << p1.x << " " << p1.y << endl;
    }
    setMouseCallback("Snooker Player Profile Management", NULL, NULL);
}

void Shot::clear()
{
    p1 = Point(-1, -1);
}

void Shot::setShotStartP(Point position)
{
    startpoint = position;
}

void Shot::drawSuggested(Mat &frame)
{
    if(startpoint.x != -1 && p1.x != -1)
    {
        line(frame, startpoint, p1, Scalar(0, 100, 0), 2, CV_AA);
        circle(frame, p1, 2, Scalar(255, 255  ,255), 2);
    }
}

double Shot::angleErr(Mat &frame, ShotArray *shot_array)
{
    //using cosine law
    angleError = abs(shot_array->angleError(p1) * 180 / M_PI);        
    stringstream ss;
    ss << "AngleError : " << angleError << " deg";
    cout << "\t\"angle_error\" : " << angleError << endl;
    putText(frame, ss.str(),
            Point(600, 600), 1, 1, Scalar(255, 255, 255), 2);
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

double Shot :: getSuggDist(){
    return (double)sqrt(pow((startpoint.x - p1.x), 2) + pow((startpoint.y - p1.y), 2)); 
}
