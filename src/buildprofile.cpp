/*!
 * \file buildprofile.cpp
 * \brief Consist methods to build user profile
 */

#include "buildprofile.hpp"
#include <cmath>
#include<stdlib.h>


using namespace std;
using namespace cv;

/*!
 *\BuildProfile constructor initialises database object,
 *\sets values of player profile to actual values from database.
 */
BuildProfile::BuildProfile(string id)
{

    string dbPath = "database/snooker.db";
    char path[dbPath.size()+1];
    dbPath.copy(path,dbPath.size(),0);
    path[dbPath.size()] = '\0';
    db = new Database(path);
    //get the values of player profile from database
    string query ="SELECT * FROM profile WHERE userID='" + id +"';";
    char temp[query.size()+1];
    query.copy(temp,query.size(),0);
    temp[query.size()] = '\0';
    vector<vector<string> > result = db->query(temp);
    for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
    {
        vector<string> row = *it;
        straight = atoi(row.at(1).c_str()); 
        cut = atoi(row.at(2).c_str());
        safety = atoi(row.at(3).c_str());
        spin = atoi(row.at(4).c_str());
        powerAcc = atoi(row.at(5).c_str());
    }

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
    int shotType = shot->shotType();  // eliminate this, use shottype variable in main, result of shot_classify.shot_classifier
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
    int overall = (straight + cut + spin + safety + powerAcc)/5;
    string query = "update profile set straight= " + to_string(straight) + ", cut= " + to_string(cut) + ", safety= " + to_string(safety) + ",spin= " + to_string(spin) + ",power= "+ to_string(powerAcc) +",overall = "+ to_string(overall) + ";";
    char temp[query.size()+1];
    query.copy(temp,query.size(),0);
    temp[query.size()] = '\0';
    // cout << query;
    db->query(temp);
    

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
