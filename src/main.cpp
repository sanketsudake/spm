/*!
  * \file main.cpp
 * \brief Consist methods to build user profile
 */

#include "detectball.hpp"
#include "detectshot.hpp"
#include "buildprofile.hpp"
#include "managelogin.hpp"
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
	CollisionDetector col_detector;
	Shot shot;

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
        int c = capture.read(src);
		if(!c)
			exit(0);

		//! detect white ball
		white_position = white_detector.detectWhite(src);
		//! Map result returned by detector
		//white_detector.mapPosition(src, white_position, 0);

		//! Used to identify first collision
		shot_detector.movingBall(src, white_position);

		//! Correct position through kalman filter
		white_position = kfchecker.correctPoisition(white_position),
		white_detector.mapPosition(src, white_position, 1);

		//! Showing accuracy
		white_accuracy.updateWithPosition(white_position);
		white_accuracy.showAccuracy(src);

		//!Call shot checker for every frame
		int trigger_val = shot_detector.shotChecker(src, white_position);
		if(trigger_val)
		{
			//Execute code
			shot_detector.preshotTrigger(src);
			white_array.clearArray();
			col_detector.drawPath(src);
			col_detector.reset();
			col_detector.setShotStartPoint(white_position);


		}

		//! White Positions array
		white_array.addPosition(white_position);
		white_array.drawPath(src);

		//! Draw suggested Point
		shot.drawSuggested(src);

		//! Draw effective collision points
		col_detector.drawPrev(src);

		//! show final image
		imshow("Snooker Player Profile Management", src);

		shot.getUserInput(src);

		if(trigger_val)
		{
			//! While ESC is not pressed dont proceed to next shot
			while(waitKey(1) != 27);
			shot.setShotStartP(white_position);
			shot.clear();

		}

		//! While ESC is not pressed dont proceed to next shot
		// Uncomment to debug code
		// while(waitKey(1) != 27);

		//! Find colliding points
		col_detector.checkCollision(white_position);

		//! Escape window on pressing 'Q' or 'q'
		code = (char)waitKey(5);
		if( code == 'q' || code == 'Q' )
			break;
    }
	return 0;
}
