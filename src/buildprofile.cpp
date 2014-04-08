/*!
 * \file buildprofile.cpp
 * \brief Consist methods to build user profile
 */

#include "buildprofile.hpp"
#include <cmath>
#include<cstdlib>
#include <sstream>


using namespace cv;
using namespace std;

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

/*!
  Profile Spec
  =============================================================================
  Profiling consist of two basic parts
  a. Ranking =>
  Just place users in ladder & find each individual's position among others.
  Create fake csv file with data input & test data.
  Data Input =>
  Input(user name, straight, cut, safety, spin, power, max pot, max score), Output(overall)
  Test Input => 
  Input(user name, straight, cut, safety, spin, power, max pot, max score), Output(overall)

  Apply Normal Equation Method to csv.
  theta = pinv(X' * X) * X' * y
  rating = input * theta

  Every time snooker program starts we should train our ranking finder with fake data.
  Then in real time we should apply to user's profile whenever above input data
  for user changes with weights we got from Normal Equation Method.

  For getting correct values for input(straight, cut, safety, ...)
  Each value should be in range 0 to 100(can be float)
  By default value should be 0.
  These values should get incremented or decremented based on users
  shot history.
  Shot classification is required for this.
  ------------------------------------------------------------------------------
  
  b. Rating => Find appropriate value for user's skills on particular scale.
  Scale should be fair to all user's
  Currently we will sort users on based overall only & rank them.
  There are many advanced cases should be considered , will take
  them after project competition.
  
  -----------------------------------------------------------------------------
 */

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
    generateThetaVals();
}

BuildProfile::~BuildProfile()
{
    db->close();
}

/*
 *Setter for setting last Frame that is to be given to/used by
 *Shot suggestion system.
 */
void BuildProfile :: setLastFrame(Mat src)
{
    lastFrame = src;
}

Mat BuildProfile :: getLastFrame()
{
    return lastFrame;;
}

void BuildProfile :: generateThetaVals()
{
    /*
      theta = pinv(X' * X) * X' * y
      rating = input * theta
      Weights will be updated in more better manner as we add more data.
      We can add or remove parameters as we like.
     */
    // Placed from data/fakeprofile.txt
    // straight,cut,safety,spin,power,maxpot,maxscore,overall/rating
    float train_data[] = {100,100,100,100,100,100,100,100,
	90.2,80,83,50,0,0,0,81.3,
	65,70,50,75,0,0,0,73,
	80,80,65,67,0,0,0,70,
	55,65,50,63,0,0,0,60,
	50,50,50,50,0,0,0,50,
	35,50,20,40,0,0,0,37,
	25,25,25,25,0,0,0,25,
	50,30,20,10,0,0,0,23,
	50,20,0,0,0,0,0,15};
    
    // Collect data from training data
    Mat data = Mat(RROWS, RCOLS, CV_32FC1, train_data).clone();
    Mat X = data.colRange(Range(0, 7)).clone();
    Mat Y =  data.colRange(Range(7, 8)).clone();

    // Small blackbox to find theta values
    Mat temp1 = Mat(RROWS, 1, CV_32FC1, Scalar(1));
    hconcat(temp1, X, X);
    Mat XTranspose = X.t();
    Mat temp2 = XTranspose * X;
    Mat temp3;
    Mat pseudo_inverted;
    invert(temp2, pseudo_inverted, cv::DECOMP_SVD);
    Mat temp4 = pseudo_inverted * XTranspose;
    theta = temp4 * Y;
    cout << theta;
}

float BuildProfile::getRating(const Mat &input)
{
    Mat temp1 = Mat(1, 1, CV_32FC1, Scalar(1));
    Mat temp2 = input.clone();
    hconcat(temp1, temp2, temp2);
    Mat result = temp2 * theta;
    return  result.at<float>(0, 0);
}

void BuildProfile :: build(double angleError, Shot *shot)
{
    int currAngleAcc = profileAngle(angleError);
    //depending on shot type
    int shotType = shot->shotType();
    // eliminate this, use shottype variable in main,
    // result of shot_classify.shot_classifier
    double dist = shot->getSuggDist();
    cout << "Dist to suggested: "<<dist << endl;

    switch(shotType)
    {
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

    //All the shot parameters should be updated before this.
    // #TODO: Replace with normal equation method
    float overall = (straight + cut + spin + safety + powerAcc)/5;
    cout << "By first method " << overall;

    // straight,cut,safety,spin,power,maxpot,maxscore
    float profile_input[] = {straight, cut, safety, spin,  powerAcc, 0, 0};
    Mat Xin = Mat(1, RCOLS - 1, CV_32FC1, profile_input);
    cout << "===== Case 1 ==== " << endl;
    cout << "Inputs => " << Xin << endl;
    overall = getRating(Xin);
    cout << "By 2nd method Rating => " << overall << endl;
    
    string query = "update profile set straight= " + patch::to_string(straight)
		+ ", cut= " + patch::to_string(cut)
		+ ", safety= " + patch::to_string(safety)
		+ ",spin= " + patch::to_string(spin)
		+ ",power= "+ patch::to_string(powerAcc)
		+ ",overall = "+ patch::to_string(overall) + ";";
    char temp[query.size()+1];
    query.copy(temp,query.size(),0);
    temp[query.size()] = '\0';
    // cout << query;
    db->query(temp);


}
void BuildProfile :: addCurrent(string userId, double angleError,
				double totalDist, double totalTime,
				double velocity ){
     string query = "insert into shothistory (userID,angleerror,totaldist,totalTime,velocity) values('" 
        + patch::to_string(userId) 
        + "','" + patch::to_string(angleError)
	 + "','" + patch::to_string(totalDist)
	 + "','" + patch::to_string(totalTime)
	 + "','" + patch::to_string(velocity)
	 + "');";
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
void BuildProfile :: profileCut(int currAngleAcc, double dist)
{

}

void BuildProfile :: profileSpin(int currAngleAcc, double dist)
{

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
