/*
 *  author: Sanket Sudake
 *  Date: 18 Aug, 2013
 *  Description:
 *  Background subtraction algorithms
 *  for moving ball detection in snooker videos
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

/* Usage :
 * $ cmake .
 * $ make
 * $ ./bgsub video.mp4
 */
int main(int argc, char *argv[])
{
	if(!argv[1])
	{
		cout << "Input video path missing" << endl;
		cout << "Sample execution" << endl;
		cout << "./bgsub video.mp4" << endl;
		exit(-1);
	}
	/* @frame - raw frame
	 * @back  - background image
	 * @fore  - foreground mask
	 */
    Mat frame, back, fore;
	VideoCapture cap(argv[1]);	// Caputure input from video
	// Read above description
	BackgroundSubtractorMOG2 bg;
	bg.set("detectShadows", false);
	bg.set("nmixtures", 3);
	bg.set("history", 30);
	// vector of vector of points
	// We connect set of detected points to contours
	// 1 vector of points = 1 contour
	// No of contours detected on fly :-)
	std::vector< std::vector<cv::Point> > contours;

	// Create two frames one to show sample video
	// other with detected motion
	namedWindow("Frame");
	//namedWindow("Background");

	for( ; ;)
	{
		// Grab frame from video at store it into frame
		cap >> frame;
		// Detect foreground objects
		// and write to fore matrix
		bg.operator()(frame, fore);
		// Detect background
		// and store in back matrix
		bg.getBackgroundImage(back);
		// Noise removal, we perform two operations
		// 1. erode
		// 2. dilate
		erode(fore, fore, Mat());
		dilate(fore, fore, Mat());
		// findcountours maps vector of points to contours
		// as described above
		findContours(fore, contours, CV_RETR_EXTERNAL,
					 CV_CHAIN_APPROX_NONE );
		// Now draw those contours on frame with red color
		vector<Point2f>center( contours.size() );
		vector<float>radius( contours.size() );
		for(int k = 0; k < contours.size(); k++)
		{
			//if(isContourConvex(contours[k]))
			{
				// Find closest circle to given contours
				// Similar to convex hull problem
				minEnclosingCircle((Mat)contours[k], center[k], radius[k]);
				// Draw circle within specified radius
				if(5 < (int)radius[k] && (int)radius[k] < 15){
					std::cout << radius[k] << std::endl;
					circle(frame, center[k], radius[k], Scalar(0, 255, 0));
				}
			}
		}

		//drawContours(frame, contours, -1, Scalar(0, 0, 255), 2);
		imshow("Frame", frame);
		//imshow("Background", back);
		if(waitKey(30) >= 0) break;
	}

    return 0;
}

/* References:
 * http://personal.ee.surrey.ac.uk/Personal/R.Bowden/
 * publications/avbs01/avbs01.pdf
 * http://mateuszstankiewicz.eu/?p=189
 * Note :
 * This is basic movement detection
 * just to showcase how we can proceed.
 * I am currently learning optical flow algorithm
 * which i think can work in our scenario
 */
