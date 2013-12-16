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

	/*!
	 * Open user input video from given path
	 * and set frame width & height.
	 */
	capture.open(argv[1]);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

	long long int frame = 0, count = 0;
	double accuracy = 0;
    while(1)
	{
        //! store image to matrix
        capture.read(src);

		//! detect white ball
		white_position = white_detector.detectWhite(src);
		//! Map result returned by detector
		white_detector.mapPosition(src, white_position);

		//! Find white position accuracy
		frame++;
		if(white_position.x == -1)
			count++;
		std::ostringstream os;
		os << "Accuracy : " << count << " / "<< frame ;
		std::string tempstring = os.str();
		// "Accuracty : " + intToString((int)accuracy) + "."
		// 		+ intToString((int)( accuracy * 1000)
		// 					  - ((int)accuracy) * 1000)
		putText(src, tempstring, Point(900, 15), 1, 1, Scalar(255, 255, 255), 2, 5);

		//! show final image
		imshow("source", src);
		waitKey(5);
    }
	return 0;
}
