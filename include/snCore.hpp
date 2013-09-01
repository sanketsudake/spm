/*
 * Filename: snCore.hpp
 * Date: August 26, 2013
 * Author: Sanket
 * Editor:
 * Description:
 * Contains all files which are in opencv liabrary.
 * Core header file for all project.
 * Header files which are written without using opencv can be
 * added here.
 */

#ifndef __SNCORE__H
#define __SNCORE__H

#include <opencv2/opencv.hpp>
// There are some header files which are not
// in opencv.hpp

using namespace cv;
using namespace std;
class snVideo
{
	VideoCapture cap;
	snVideo(char *filename);
	snVideo(int no);
	~snVideo();
	void getframe(Mat *frame);
};
class snBall
{
	float xpos;
	float ypos;
	float radius;
	/*
	  No  Color    Points
	  15   RED     1
	  1    YELLOW  2
	  1    GREEN   3
	  1    BROWN   4
	  1	   BLUE    5
	  1	   PINK    6
	  1	   BLACK   7
	  1    WHITE   CUE
	*/
	enum ballcolor {
		RED,
		YELLOW,
		GREEN,
		BROWN,
		BLUE,
		PINK,
		BLACK,
		WHITE,
	}color;
	int ismoving;

	snBall(float , float ,snBall::ballcolor ) ;
	~snBall();
	void updatepos(float x, float y);
};
#endif
