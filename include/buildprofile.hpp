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
#define RROWS 11
#define RCOLS 8

/*!
 * \class BuildProfile
 * \brief
 */

class BuildProfile
{
    private:
        float angleAcc;
        float powerAcc;
        float straight;
        float spin;
        float cut;
        float safety;
        Mat lastFrame, theta;
        Database *db;
    public:
        BuildProfile(string id);
        ~BuildProfile();
        void setLastFrame(Mat);
        Mat getLastFrame();
        int profileAngle(double);
        void generateThetaVals();
        float getRating(const Mat &input);
        int getRanking(const char *uname);
        void build(double, Shot *);
        void addCurrent(string,double,double,double,double, int);
        void profileStraight(int currAngleAcc, double  );
        void profileSpin(int currAngleAcc, double  );
        void profileCut(int currAngleAcc, double  );
};

#endif
