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
#include "database.hpp"
#include "shot.hpp"
#include <string>
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
        int safety;
        Mat lastFrame;
        Database *db;
    public:
        BuildProfile(string id);
        ~BuildProfile();
        void setLastFrame(Mat);
        Mat getLastFrame();
        int profileAngle(double);
        void build(double, Shot *);
        void addCurrent(double,double,double,double);
        void profileStraight(int currAngleAcc, double  );
        void profileSpin(int currAngleAcc, double  );
        void profileCut(int currAngleAcc, double  );
};


#endif
