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

class snVideo
{
	VideoCapture cap;
	snVideo(char *filename);
	snVideo(int no);
	~snVideo();
	void getframe(Mat *frame);
};

#endif
