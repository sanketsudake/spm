#include<snCore.hpp>
#include<ballDetect.h>
#include<iostream>
using namespace cv;
using namespace std;

/*
 *Arguments:
 *argc
 *argv:
 *Description:
 *Start point for snooker project.
 */

int main(int argc, char **argv)
{
	if(!argv[1])
		cout << "Video argument missing" << endl;
    ballDetect detect;
    detect.initDetect(argv[1]);
	return 0;
}
