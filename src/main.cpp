/*!
 * \file main.cpp
 * \brief Consist methods to build user profile
 */

#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	VideoCapture capture;		//! Videocapture to capture video object
    Mat src;					//! Matrix object to get input

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


		//! show final image
		imshow("source", src);
		waitKey(5);
    }
	return 0;
}
