/*!
 * \file buildprofile.cpp
 * \brief Consist methods to build user profile
 */

#include "buildprofile.hpp"
#include <cmath>
using namespace std;
using namespace cv;

BuildProfile::BuildProfile()
{
     angleAcc = 100;
     powerAcc = 100;
     straight = 100;
     spin = 100;
     cut = 100;
}
BuildProfile::~BuildProfile()
{

}
void BuildProfile :: build(double angleError){
  double angleAcc = profileAngle(angleError);



}


//this factor will be considered irrespective of shot type.
int BuildProfile :: profileAngle(double angleError){
    double currentAngleAcc = exp(-angleError);
    currentAngleAcc *= 100; 
    //getSuggestedDist();
    //cout << "curre: " << << endl;
    angleAcc = (currentAngleAcc + angleAcc)/2;
     cout << "Profile Angle Accuracy: " << angleAcc << endl;
    return currentAngleAcc;
}


