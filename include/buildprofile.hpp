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
public:
	BuildProfile();
	~BuildProfile();
        int profileAngle(double);
        void build(double);
};


#endif
