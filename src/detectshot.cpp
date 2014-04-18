/*!
 * \file detectshot.cpp
 * \brief Consist methods to process shot
 */

#include "detectshot.hpp"
#include <cmath>
#include <vector>

using namespace std;
using namespace cv;

extern Mat roi_image;

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


int DetectShot::shotChecker(Mat &frame, int *count, Point position)
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
    // displayShotnumber(game_statistics);
    *count = shotcount;

    return returnval;
}


void DetectShot::preshotTrigger(Mat &frame)
{
    // putText(frame, "Press ESC.",
    //         Point(600, 500), 1, 1, Scalar(200, 200, 200), 2);
}


void DetectShot::shotTrigger(Mat &frame)
{
    shotcount++;
    if(shotcount<=1)
        roi_image = imread("./media/whiteBall.png", CV_LOAD_IMAGE_COLOR);
    else{
        roi_image = imread("./media/noSpin.png", CV_LOAD_IMAGE_COLOR);
    }
    cv::resize(roi_image, roi_image, Size(200, 200), 2, 2, INTER_CUBIC);
}


void DetectShot::displayShotnumber(Mat &frame)
{
    //! Show shot number on screen
    stringstream ss;
    ss << " " << shotcount;
    // putText(frame, ss.str(), Point(40, 15), 1, 1, Scalar(255, 255, 255), 2);
    putText(frame, ss.str(), Point(frame.cols/2+10, frame.rows/4*3-25), 1, 1, Scalar(0, 0, 0), 2);
}


int DetectShot::isWhiteMoving(Point position)
{
    /*! Check is ball is moving with previous position
     * Returns 0 if ball not moving
     * Returns 1 if ball is moving
     */

    if(prevPosition.x == -1)
        return 0;

    else if(std::abs(position.x - prevPosition.x) == 0
                && std::abs(position.y - prevPosition.y) == 0)
            return 0;

    return 1;
}
