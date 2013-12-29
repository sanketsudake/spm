/*!
 * \file buildprofile.cpp
 * \brief Consist methods to build user profile
 */

#include "buildprofile.hpp"

using namespace std;
using namespace cv;

void onMouseClick(int event, int x, int y, int flags, void* userdata)
{
	Point *p = ((Point *)userdata);
    if(event == EVENT_LBUTTONDOWN)
	{
        cout << "Mouseclick " << x << " " << y << endl;
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

		if(p1.x != -1)
			cout << "Got value " << p1.x << " " << p1.y << endl;
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

void Shot::angleError()
{

}

void Shot::velocityError()
{

}

void Shot::rateShot()
{

}

void Shot::shottype()
{

}

BuildProfile::BuildProfile()
{

}
BuildProfile::~BuildProfile()
{

}
