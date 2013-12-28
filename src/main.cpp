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
	Point prev(0, 0);
	double slope2 = 1000;

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
		shot_detector.shotChecker(src, &white_array, white_position);

		//! White Positions array
		white_array.addPosition(white_position);
		white_array.drawPath(src);

		circle(src, prev, 2, Scalar(255, 0 ,0), 2);

		//! show final image
		imshow("Snooker Player Profile Management", src);

		double slope1 = 0;
		//! Temporary code need to removed
		cout << "White Position " << white_position.x
			 << " "
			 << white_position.y << " ";
		cout << prev.y - white_position.y << " "
			 << prev.x - white_position.x << " ";

		if(prev.x - white_position.x)
		{
			slope1 = ((double)(prev.y - white_position.y)/(prev.x - white_position.x));
			cout << slope1 <<  " ";
			cout << slope2 - slope1 ;
			if(slope2 < 900)
			{
				if(abs(slope2 - slope1) > 0.25)
				{
					if(abs(prev.y - white_position.y) == 1 && abs(prev.x - white_position.x) == 1)
						cout << endl;
					else
					{
						cout << "Collision" << endl;
						//! While ESC is not pressed dont proceed to next shot
						while(waitKey(1) != 27);
					}
				}
				else
					cout << endl;
			}
			else
				cout << endl;
		}
		else
		{
			slope1 = 1000;
			cout << endl;
		}

		prev = white_position;
		slope2 = slope1;

		//! Escape window on pressing 'Q' or 'q'
		code = (char)waitKey(5);
		if( code == 'q' || code == 'Q' )
			break;
    }
	return 0;
}
