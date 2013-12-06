#ifndef __BALLDETECT_H_INCLUDED__
#define __BALLDETECT_H_INCLUDED__

#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<string>

using namespace cv;

class ballDetect{
    private:
        // int shot=0;
        // max number of objects to be detected in frame
        static const int MAX_NUM_OBJECTS = 50;
        //minimum and maximum object area
        static const int MIN_OBJECT_AREA = 5*5;
        static const int MAX_OBJECT_AREA = 20 * 20;
        //FRAME_HEIGHT*FRAME_WIDTH/1.5;
        //default capture width and height
        static const int FRAME_WIDTH = 640;
        static const int FRAME_HEIGHT = 480;
        Scalar *minval,*maxval;

    public:
        ballDetect();
        string intToString(int number);
        void drawObject(int x, int y,Mat &frame,int ballIndex,int redIndex);
        void morphOps(Mat &processed);
        void trackFilteredObject(int &, int &, Mat , Mat &,int);
        void initDetect(char *);
        void detectShot();
};

#endif
