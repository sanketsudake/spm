/*!
 * \file detectshot.cpp
 * \brief Consist methods to process shot
 */

#include "detectshot.hpp"

using namespace std;
using namespace cv;

ShotArray::ShotArray()
{
	white_positions.clear();
}

ShotArray::~ShotArray()
{

}

void ShotArray::addPosition(Point position)
{
	white_positions.push_back(position);
}

void ShotArray::clearArray()
{
	white_positions.clear();
}

void ShotArray::drawPath(Mat &frame)
{
	int white_positions_size = (int)white_positions.size();
	for(int i = 0; i < white_positions_size - 1; i++ )
	{
		line(frame, white_positions[i], white_positions[i+1],
			 Scalar(255, 255, 255), 1, CV_AA, 0);
		circle(frame, white_positions[i], 1, Scalar(255, 255 ,200), 2);
	}
}

DetectShot::DetectShot()
{
	shotcount = 0;
	shot_state = 0;
	prevPosition = Point(-1, -1);
	shottemp = 0;
    // Innital tweaks required for easy shot identification
    cout << "Adjust SHOT_TEMP_COUNT if shot start & end not identified" << endl
         << "correctly.Standard time interval should be kept bet. two" << endl
         << "shots." << endl;
}

DetectShot::~DetectShot()
{

}

int DetectShot::BgSubtractor(Mat &frame)
{
	//! Perform background subtraction idenfies foreground objects
	bg.operator ()(frame, fore);
	bg.getBackgroundImage(back);

    //! Erode and dilate objects on foreground objects
	erode(fore, fore, Mat());
	dilate(fore, fore, Mat());

	//! Find contours on foreground image
	findContours(fore, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//drawContours(frame,  contours,  -1,  Scalar(0,  0,  255),  2);

	//! Return contours size
	return contours.size();
}

void DetectShot::shotChecker(Mat &frame, ShotArray *shotarray)
{
	//! Get no of contours first
	int contours_size = BgSubtractor(frame);

	/*! Identify state of shot in automata
     * shot_state contais two states
     * shot_state 0: shot is going on.
     * shot_state 1: shot ends(no contours movement).
     * Fires shotTrigger when SHOT_TEMP_COUNT reached.
     * in shot_state 1 reached.
     */
	switch(shot_state)
	{
	case 0:
        //! if contours_size state changes to 1
		if(contours_size == 0)
		{
			shot_state = 1;
			shottemp = 1;
		}
		break;
	case 1:
        //! if contours_size state changes to 0
		if(contours_size > 0)
		{
			shot_state = 0;
            /*! shows max possible value of shottemp between
             * shot state change. Helps to decide value
             * of SHOT_TEMP_COUNT
             */
			cout << "shotChecker:shottemp " << shottemp << endl;
		}
		else
		{
			shottemp++;
			if(shottemp == SHOT_TEMP_COUNT - 1)
				preshotTrigger(frame);
            //! When shot temp reached fire shotTrigger
			if(shottemp == SHOT_TEMP_COUNT)
				shotTrigger(frame, shotarray);
		}
		break;
	default:
		exit(0);
	}
	displayShotnumber(frame);
}

void DetectShot::preshotTrigger(Mat &frame)
{
	putText(frame, "Press ESC.",
			Point(600, 500), 1, 1, Scalar(200, 200, 200), 2);
}

void DetectShot::shotTrigger(Mat &frame, ShotArray *shotarray)
{
	shotcount++;
	shotarray->clearArray();
	//! While ESC is not pressed dont proceed to next shot
	while(waitKey(1) != 27);
}

void DetectShot::displayShotnumber(Mat &frame)
{
	//! Show shot number on screen
	stringstream ss;
	ss << "Shot No : " << shotcount;
	putText(frame, ss.str(),
			Point(40, 15), 1, 1, Scalar(255, 255, 255), 2);
}
