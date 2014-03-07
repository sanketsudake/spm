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
    db = new Database("database/snooker.db");
}

BuildProfile::~BuildProfile()
{
    db->close();    
}
/*
 *Setter for setting last Frame that is to be given to/used by 
 *Shot suggestion system.
 */
void BuildProfile :: setLastFrame(Mat src){
    lastFrame = src;
}
Mat BuildProfile :: getLastFrame(){
    return lastFrame;;
}
void BuildProfile :: build(double angleError, Shot *shot){
    int currAngleAcc = profileAngle(angleError);
    //depending on shot type
    int shotType = shot->shotType();
    double dist = shot->getSuggDist();
    cout << "Dist to suggested: "<<dist << endl;

    switch(shotType){
        //if straight shot
        case 1: profileStraight(currAngleAcc, dist);
                break;
                //if cut shot
        case 2: profileCut(currAngleAcc, dist);
                break;
                //if spin 
        case 3: profileSpin(currAngleAcc,dist);
                break;
    }
}

void BuildProfile :: profileStraight(int currAngleAcc, double dist){
    //max dist can be 1101,  
    dist = dist /500;
    dist = 1 + (exp(-dist) / 10);
    double currentStraight = currAngleAcc * dist;
    //need to think of good logic for updating score #change
    straight = (straight + currentStraight)/2; 
    //add straight to database
    
    cout << "Current Straight Accuracy: " << currentStraight << endl;
    cout << "Profile Straight Accuracy: " << straight <<  "  dist" << dist;
    return;
}
void BuildProfile :: profileCut(int currAngleAcc, double dist){

}

void BuildProfile :: profileSpin(int currAngleAcc, double dist){

}



//this factor will be considered irrespective of shot type.
int BuildProfile :: profileAngle(double angleError){
    double currentAngleAcc = exp(-angleError);
    currentAngleAcc *= 100; 
    cout << "Current Angle Accuracy: " << currentAngleAcc<< endl;
    angleAcc = (currentAngleAcc + angleAcc)/2;
    cout << "Profile Angle Accuracy: " << angleAcc << endl;
    return currentAngleAcc;
}

