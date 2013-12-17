/*!
 * \file main.cpp
 * \brief Consist methods to build user profile
 */

#include "detectball.hpp"
#include "detectshot.hpp"
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
	SnKalman kfchecker;
	DetectShot shot_detector;
	ShotArray white_array;

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
		//white_detector.mapPosition(src, white_position, 0);

		//! Correct position through kalman filter
		white_position = kfchecker.correctPoisition(white_position),
		white_detector.mapPosition(src, white_position, 1);

		//! Showing accuracy
		white_accuracy.updateWithPosition(white_position);
		white_accuracy.showAccuracy(src);

		//!Call shot checker for every frame
		shot_detector.shotChecker(src, &white_array);

		//! White Positions array
		white_array.addPosition(white_position);
		white_array.drawPath(src);

		//! show final image
		imshow("Snooker Player Profile Management", src);

		//! Escape window on pressing 'Q' or 'q'
		code = (char)waitKey(5);
		if( code == 'q' || code == 'Q' )
			break;
    }
	return 0;
}
