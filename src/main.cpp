/*!
 * \file main.cpp
 * \brief Consist methods to build user profile
 */

#include "detectball.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <ctime>
#include <cstdio>
#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	VideoCapture capture;		//! Videocapture to capture video object
    Mat src;					//! Matrix object to get input
	Point white_position(-1, -1); //! White Ball Position
	DetectBall white_detector;
	char code = (char)-1;
	BallAccuracy white_accuracy;
	/*!
	 * Open user input video from given path
	 * and set frame width & height.
	 */
	capture.open(argv[1]);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

    while(1)
	{
        //! store image to matrix
        capture.read(src);

		//! detect white ball
		white_position = white_detector.detectWhite(src);
		//! Map result returned by detector
		white_detector.mapPosition(src, white_position);

		//! Showing accuracy
		white_accuracy.updateWithPosition(white_position);
		white_accuracy.showAccuracy(src);

		//! show final image
		imshow("Snooker Player Profile Management", src);

		// waitKey(5);
		//! Escape window on pressing 'Q' or 'q'
		code = (char)waitKey(5);
		if( code == 'q' || code == 'Q' )
			break;
    }
	return 0;
}
