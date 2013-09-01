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
using namespace std;
snVideo::snVideo(char *filename)
{
	assert(filename);
	cap = VideoCapture(filename);
	assert(cap.isOpened());
}

snVideo::snVideo(int no)
{
	cap = VideoCapture(no);
	assert(cap.isOpened());
}

snVideo::~snVideo()
{
	delete &cap;
}
snBall::snBall(float x, float y,ballcolor c): color(c)
{
	xpos = x;
	ypos = y;
	color = c;
	ismoving = 0;
	radius = 0;
}
snBall::~snBall()
{

}
void snBall::updatepos(float x, float y)
{

}
