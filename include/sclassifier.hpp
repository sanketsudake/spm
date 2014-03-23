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

enum shot_type{
    //! Template
    // SPIN TYPE + White final on which side of tangent

    NO_SPIN_HEADON,
    CUT_SHOT,

    FORWARD_SPIN_HEADON,
    FORWARD_SPIN_LEFT,
    FOWARD_SPIN_RIGHT,

    BACKWARD_SPIN_HEADON,
    BACKWARD_SPIN_LEFT,
    BACKWARD_SPIN_RIGHT,

    LEFT_SPIN_HEADON,
    LEFT_SPIN_LEFT,
    LEFT_SPIN_RIGHT,

    RIGHT_SPIN_HEADON,
    RIGHT_SPIN_LEFT,
    RIGHT_SPIN_RIGHT
};

class Sclassifier
{
private:
    shot_type shot_class;
public:
    Sclassifier();
    ~Sclassifier();
    int classifier(int beta, Point contact, int alpha);
};

#endif
