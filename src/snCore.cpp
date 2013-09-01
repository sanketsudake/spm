 /*
 * Filename: snCore.cpp
 * Date: August 28, 2013
 * Author: Sanket
 * Editor:
 * Description:
 * Core functions in opencv redefined
 */

#include"snCore.hpp"
#include<iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

snVideo::snVideo(char *filename)
{
	assert(filename);
	cap = VideoCapture(filename);
	assert(cap.isOpened());
}

snVideo::snVideo(int no)
{
	assert(!cap);
	cap = VideoCapture(no);
	assert(cap.isOpened());
}

~snVideo()
{
	delete cap;
}
