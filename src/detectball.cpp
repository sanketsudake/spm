/*!
 * \file detectball.cpp
 * \brief Consist methods to detect ball
 */

#include "detectball.hpp"


using namespace std;
using namespace cv;

DetectBall::DetectBall()
{
	//! HSV Min and Max Range for white ball detection
	white_minval = new Scalar(0, 0, 200);
	white_maxval = new Scalar(255, 70, 255);
    frameCount = 0;
}
DetectBall::~DetectBall()
{

}

void DetectBall :: showFrameNo(Mat &frame)
{
    frameCount++;
    stringstream ss;
	ss << "Frame No : " << frameCount;
	putText(frame, ss.str(), Point(900, 15), 1, 1, Scalar(255, 255, 255), 2);

}

void DetectBall::morphOps(Mat &frame)
{
	/*! create structuring element that will be used to
	 * "dilate" and "erode" image.
     * the element chosen here is a 3px by 3px rectangle
	 */
    Mat erodeElement = getStructuringElement( MORPH_ELLIPSE, Size(3,3));
    //! dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement( MORPH_ELLIPSE, Size(7,7));

	//! erode removes noise from identified contours
    erode(frame, frame, erodeElement);
    erode(frame, frame, erodeElement);

	//! dilate adjusts shape of identified object
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

void DetectBall::mapPosition(Mat &frame, Point position, int status)
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
		//! check status point coming from white detector or kalman fiter
		if(!status)
			circle(frame, position, 12, Scalar(0, 0, 0), 2);
		else
			circle(frame, position, 10, Scalar(255, 255, 255), 2);
	}
}

SnKalman:: SnKalman()
{
	//! Specify dimensions of kalman filter
	kalmanfilter.init(4, 2, 0);
	state = new Mat_<float>(4,1);
	processNoise = new Mat(4, 1, CV_32F);
	measurement = new Mat_<float>(2,1);
	(*measurement).setTo(Scalar(0));
	flaginit = 1;
}

SnKalman:: ~SnKalman()
{

}

Point SnKalman::correctPoisition(Point position)
{
	/*! Innitiate kalmanfilter at current state
	 *  Adjust process noise and error noise
	 */
	if(flaginit)
	{
		kalmanfilter.statePre.at<float>(0) = position.x;
		kalmanfilter.statePre.at<float>(1) = position.y;
		kalmanfilter.statePre.at<float>(2) = 0;
		kalmanfilter.statePre.at<float>(3) = 0;
		kalmanfilter.transitionMatrix = *(Mat_<float>(4, 4)
										  << 1,0,0,0,
										     0,1,0,0,
										     1,0,1,0,
										     0,1,0,1);
		setIdentity(kalmanfilter.measurementMatrix);
		setIdentity(kalmanfilter.processNoiseCov, Scalar::all(1e-1));
		setIdentity(kalmanfilter.measurementNoiseCov, Scalar::all(1e-5));
		setIdentity(kalmanfilter.errorCovPost, Scalar::all(.1));
		flaginit = 0;
		return position;
	}
	else
	{
		/*!
		 * Kalman filter consist of two main states
		 * 1. Prediction
		 * 2. Correction
		 * Below code returns estimated point
		 */
		// Point statePt(state(0),state(1));
		Mat prediction = kalmanfilter.predict();
		Point predictPt(prediction.at<float>(0),prediction.at<float>(1));

		//! if position measurement correct
		if(position.x != -1)
		{
			(*measurement)(0) = position.x;
			(*measurement)(1) = position.y;
		}
		Point measPt((*measurement)(0), (*measurement)(1));

		//! generate measurement
		//! measurement += KF.measurementMatrix*state;
		Mat estimated = kalmanfilter.correct(*measurement);
		Point statePt(estimated.at<float>(0),estimated.at<float>(1));
		if(position.x != -1)
			return position;
		return Point(estimated.at<float>(0), estimated.at<float>(1));
	}
}
