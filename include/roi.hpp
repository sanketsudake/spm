#ifndef __ROI_H_INCLUDED__
#define __ROI_H_INCLUDED__
#include <sstream>
#include <string>
#include <iostream>
#include <opencv/highgui.h>
#include <opencv/cv.h>

using namespace cv;

class Roi
{
    private:
        bool gotroi;
        vector<Vec3f> final_roi;
    public:
        Roi();
        ~Roi();
        bool gotRoi();
        void morphOps(Mat &thresh);
        vector<Vec3f>& getRoi(Mat camerafeed);
};

#endif
