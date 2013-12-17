/*!
 * \file detectshot.cpp
 * \brief Consist methods to process shot
 */

#include "detectshot.hpp"

using namespace std;
using namespace cv;

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

void DetectShot::shotChecker(Mat &frame)
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
            //! When shot temp reached fire shotTrigger
			if(shottemp == SHOT_TEMP_COUNT)
				shotTrigger(frame);
		}
		break;
	default:
		exit(0);
	}
	displayShotnumber(frame);
}

void DetectShot::shotTrigger(Mat &frame)
{
	shotcount++;
    putText(frame, "Press ESC.",
			Point(600, 500), 1, 1, Scalar(200, 200, 200), 2);
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
