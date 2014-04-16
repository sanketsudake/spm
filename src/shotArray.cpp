/*!
 * \file shotArray.cpp
 * \brief consist of vital parametes & functions
 * required for shot.
 */

#include "../include/shotArray.hpp"
#include <cmath>
#include <vector>
using namespace std;
using namespace cv;

ShotArray::ShotArray()
{
    white_positions.clear();
    shot_start = time(NULL);
    prevPosition = Point(0,0);
}

ShotArray::~ShotArray()
{

}

void ShotArray::addPosition(Point position)
{
    //set the previous position if it is not set
    if(!prevPosition.x && !prevPosition.y){
        prevPosition = position;
        white_positions.push_back(position);

    }
    else if((prevPosition.x - position.x) && (prevPosition.y - position.y)){
        white_positions.push_back(position);
        prevPosition = position;
    }
}

void ShotArray::clearArray()
{
    white_positions.clear();
    shot_start = time(NULL);
    prevPosition = Point(0,0);
}

void ShotArray::drawPath(Mat &frame)
{
    int white_positions_size = (int)white_positions.size();
    for(int i = 0; i < white_positions_size - 1; i++ )
    {
        line(frame, white_positions[i], white_positions[i+1],
                Scalar(255, 255, 255), 1, CV_AA, 0);
        circle(frame, white_positions[i], 1, Scalar(255, 255 ,200), 2);
    }
}

double ShotArray::dist(Point p1, Point p2)
{
    return (double)sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}

double ShotArray::totalDist()
{
    double tot_dist = 0;
    for(int i = 0; i < white_positions.size() - 1; i++)
    {
        tot_dist += dist(white_positions[i], white_positions[i+1]);
    }
    return tot_dist;
}

double ShotArray :: angleError(Point C)
{
    double b = dist(C,white_positions[1]);
    double c = dist(white_positions[1], white_positions[0]);
    double a = dist(white_positions[0], C);
    return acos(((a*a) + (c*c) - (b*b))/(2*a*c));
}

double ShotArray::totalTime()
{
    return (double)difftime(time(NULL), shot_start);
}

double ShotArray::shotVelocity()
{
    return (double)totalDist()/totalTime();
}

