/*!
 * \file detectball.cpp
 * \brief Consist methods to detect ball
 */

#include "detectball.hpp"
#include <iostream>
using namespace std;
using namespace cv;

DetectBall::DetectBall()
{
	white_minval = new Scalar(0, 0, 200);
	white_maxval = new Scalar(255, 70, 255);
}
DetectBall::~DetectBall()
{

}

void DetectBall::morphOps(Mat &frame)
{
	/*!create structuring element that will be used to
	 *"dilate" and "erode" image.
     *the element chosen here is a 3px by 3px rectangle
	 */
    Mat erodeElement = getStructuringElement( MORPH_ELLIPSE, Size(3,3));
    //! dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement( MORPH_ELLIPSE, Size(7,7));

    erode(frame, frame, erodeElement);
    erode(frame, frame, erodeElement);

    dilate(frame, frame, dilateElement);
    dilate(frame, frame, dilateElement);
}

Point DetectBall::trackFilteredObject(Mat &frame)
{
	//! two vectors needed for output of find contours
	vector< vector<Point> > contours;
	vector<Vec4i>  hierarchy;
	int x = 0, y = 0;

	//! find contours of filetered image using OpenCv function
	findContours(frame, contours, hierarchy, CV_RETR_CCOMP,
				 CV_CHAIN_APPROX_SIMPLE );

	//! use moments to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0)
	{
		int numObjects = hierarchy.size();
		//! if number of objects greater than MAX_NUM_OBJECTS,apply noisy filter
		if(numObjects < MAX_NUM_OBJECTS)
		{
			for (int index = 0; index >= 0; index = hierarchy[index][0])
			{
				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;
				// cout << area << endl;
				/*! if the area is less than 20 px by 20px then it is probably
				 * just noise if the area is the same as the 3/2 of the image size,
				 * probably just a bad filter we only want the object with the
				 * largest area so we safe a reference area each iteration and
				 * compare it to the area in the next iteration.
				 */
				if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea)
				{
					x = moment.m10/area;
					y = moment.m01/area;
					objectFound = true;
				}
				else
					objectFound = false;
			}
			//! let user know you found an object
			if(objectFound == true)
			{
				// cout << "White Ball at: " << x << y << endl;
				//! return object location on screen
				return Point(x, y);
			}
		}
		else
		{
			//! Error:TOO MUCH NOISE! ADJUST FILTER
			return Point(-2, -2);
		}
	}
	//! White ball Position not found
	return Point(-1, -1);
}

Point DetectBall::detectWhite(Mat &frame)
{
	Mat frame_HSV, processed;
	//! convert frame from BGR to HSV colorspace
	cvtColor(frame, frame_HSV, COLOR_BGR2HSV);

	//! Apply threshold to detect white ball
	inRange(frame_HSV, Scalar(0, 0, 200), Scalar(255, 70, 255), processed);

	//! Apply morphological operations
	 morphOps(processed);

	 //! tracking objects from filtered object
	 return trackFilteredObject(processed);
}

void DetectBall::mapPosition(Mat &frame, Point position)
{
	//! Show actions according position found by detectBall
	switch(position.x)
	{
		//! Failed to find position
		//! Mostly occurs when Foul appears on screen
	case -1:
		putText(frame, "Failed to Find Position",
					 Point(0, 50), 1, 2, Scalar(0, 0, 255),2);
		putText(frame, "Possibly Foul",
					 Point(0, 100), 1, 2, Scalar(0, 0, 255),2);
		break;
		//! Less possible occurance of this case
	case -2: putText(frame, "TOO MUCH NOISE! ADJUST FILTER",
					 Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
		break;
	default:
		circle(frame, position, 10, Scalar(0,255,0), 2);
	}
}
