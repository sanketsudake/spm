#ifndef __GUI_H_INCLUDED__
#define __GUI_H_INCLUDED__

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <string>

//Hard-code these values for your configuration
#define SCREEN_WIDTH 1440 
#define SCREEN_HEIGHT 900 

using namespace std;
using namespace cv;

class GUI 
{
	private:
        Mat profile_picture;
        Mat profile_table;
        Mat game_statistics;
        void prepare_game_statistics(string, Mat );
        void prepare_profile_table();

	public:
        int frame_count;
        int shot_count;
        // All parameters of profile
        // write to these variables and it will display in profile_table window.

	    GUI();
	    ~GUI();
        void ShowProfilePicture();
        void ShowProfileTable();
        void ShowGameStatistics(string );
        void GetGreenMat(Mat );
};

#endif
