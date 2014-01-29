/*!
 * \file buildprofile.hpp
 * \brief
 */

#ifndef __BUILDPROFILE_H_INCLUDED__
#define __BUILDPROFILE_H_INCLUDED__
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<iostream>
#include <cmath>
#include "detectshot.hpp"
#include "shot.hpp"
using namespace std;
using namespace cv;
/*!
 * \class BuildProfile
 * \brief
 o */
class BuildProfile
{
    private:
        int angleAcc;
        int powerAcc;
        int straight;
        int spin;
        int cut;
        Mat lastFrame;
    public:
        BuildProfile();
        ~BuildProfile();
        void setLastFrame(Mat);
        Mat getLastFrame();
        int profileAngle(double);
        void build(double, Shot *);
        void profileStraight(int currAngleAcc, double  );
        void profileSpin(int currAngleAcc, double  );
        void profileCut(int currAngleAcc, double  );
};


#endif
