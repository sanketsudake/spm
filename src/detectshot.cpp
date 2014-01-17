/*!
 * \file detectshot.cpp
 * \brief Consist methods to process shot
 */

#include "detectshot.hpp"
#include <math.h>
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

DetectShot::DetectShot()
{
    shotcount = 0;
    shot_state = 0;
    shotstart = 0;
    prevPosition = Point(-1, -1);
    shottemp = 0;
    flag = 1;
    contour_temp = 0;
    // Innital tweaks required for easy shot identification
    // cout << "Adjust SHOT_TEMP_COUNT if shot start & end not identified" << endl
    //      << "correctly.Standard time interval should be kept bet. two" << endl
    //      << "shots." << endl;
}

DetectShot::~DetectShot()
{

}

int DetectShot::BgSubtractor(Mat &frame)
{
    //! Perform background subtraction idenfies foreground objects
    bg.operator ()(frame, fore);
    bg.getBackgroundImage(back);

    //! Erode and dilate objects on foreground objects
    erode(fore, fore, Mat());
    dilate(fore, fore, Mat());

    //! Find contours on foreground image
    findContours(fore, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    //drawContours(frame,  contours,  -1,  Scalar(0,  0,  255),  2);

    //! Return contours size
    return contours.size();
}

int DetectShot::shotChecker(Mat &frame, Point position)
{
    int returnval = 0;

    //! Get no of contours first
    int contours_size = BgSubtractor(frame);

    //! Is white ball moving
    int whiteMoving = isWhiteMoving(position);
    prevPosition = position;
    if(!shotcount)
    {
        if(flag == 2)
        {
            shotTrigger(frame);
            returnval = 1;
            flag = 0;
        }
        if(flag)
        {
            //preshotTrigger(frame);
            flag = 2;
        }
    }

    //! Let ball five times => shot start
    if(whiteMoving && !flag)
    {
        shottemp++;
        if(shottemp > 5)
            flag = 1;
    }

    //! Shot end => count background subtraction frames
    if(contours_size == 0 && flag)
    {
        contour_temp++;
        // if(contour_temp == 9)
        // 	preshotTrigger(frame);
        if(contour_temp == 10)
        {
            returnval = 1;
            shotTrigger(frame);
            // cout << "Contour" << contour_temp << shottemp << endl;
            flag = contour_temp = shottemp = 0;
        }
    }
    displayShotnumber(frame);

    return returnval;
}

void DetectShot::preshotTrigger(Mat &frame)
{
    putText(frame, "Press ESC.",
            Point(600, 500), 1, 1, Scalar(200, 200, 200), 2);
}

void DetectShot::shotTrigger(Mat &frame)
{
    shotcount++;
}

void DetectShot::displayShotnumber(Mat &frame)
{
    //! Show shot number on screen
    stringstream ss;
    ss << "Shot No : " << shotcount;
    putText(frame, ss.str(),
            Point(40, 15), 1, 1, Scalar(255, 255, 255), 2);
}

int DetectShot::isWhiteMoving(Point position)
{
    /*! Check is ball is moving with previous position
     * Returns 0 if ball not moving
     * Returns 1 if ball is moving
     */
    if(prevPosition.x == -1)
        return 0;
    else
    {
        if(std::abs(position.x - prevPosition.x) == 0
                && std::abs(position.y - prevPosition.y) == 0)
            return 0;
    }
    return 1;
}



