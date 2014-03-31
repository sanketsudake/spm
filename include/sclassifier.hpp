#ifndef __SCLASSIFIER_H_INCLUDED__
#define __SCLASSIFIER_H_INCLUDED__
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

class Sclassfier
{
    private:
       int alpha, beta;
       Point contact;

    public:
       Sclassfier();
       ~Sclassfier();
       int classfier(int alpha, int beta, Point contact);
};

#endif
