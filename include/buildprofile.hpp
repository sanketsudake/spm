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
#define ANGLE_RANGE 180.0
#define DISTANCE_RANGE 1100.0

/*!
 * \class BuildProfile
 * \brief
 */

class BuildProfile
{
    private:
        float powerAcc;
        float straight;
        float spin;
        float cut;
        float safety;
        float overall;
        float maxpot;
        float maxscore;
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
        void build(string, int);
        void addCurrent(string,double,double,
			double,double,double, int);
};

#endif
