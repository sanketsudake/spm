/*!
 * \file main.cpp
 * \brief Consist methods to build user profile
 */

#include "detectball.hpp"
#include "detectshot.hpp"
#include "buildprofile.hpp"
#include "managelogin.hpp"
#include "shot.hpp"
#include "shotArray.hpp"
#include "collisionDetector.hpp"
#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    VideoCapture capture;		//! Videocapture to capture video object
    Mat src,prev;					//! Matrix object to get input
    Point white_position(-1, -1); //! White Ball Position
    DetectBall white_detector;
    char code = (char)-1;

    SnKalman kfchecker;
    DetectShot shot_detector;
    ShotArray white_array;
    CollisionDetector col_detector;
    Shot shot;
    BuildProfile build_profile;
    int flag = 1;
    /*!
     * Open user input video from given path
     * and set frame width & height.
     */
    capture.open(argv[1]);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

    while(1)
    {
        //! store image to matrix
        int c = capture.read(src);
        if(!c)
            exit(0);

        //! detect white ball
        white_position = white_detector.detectWhite(src);
        //! Map result returned by detector
        //white_detector.mapPosition(src, white_position, 0);


        //! Correct position through kalman filter
        white_position = kfchecker.correctPoisition(white_position),
                       white_detector.mapPosition(src, white_position, 1);

        //! Display frame number
        white_detector.showFrameNo(src);

        //!Call shot checker for every frame
        int trigger_val = shot_detector.shotChecker(src, white_position);
        if(trigger_val)
        {
            //Execute code
            // Do not if block for first shot starting
            if(!flag)
            {
                cout << "\t\"total_distance\" : " << white_array.totalDist() << ","<< endl;
                cout << "\t\"total_time\" : " << white_array.totalTime() << ","<< endl;
                // Velocity in cm/sec
                cout << "\t\"velocity\" : " << white_array.shotVelocity() * (0.367347)  << "," << endl;
                shot_detector.preshotTrigger(src);
                double angleError = shot.angleErr(src, &white_array);
                //int currAngleAcc = build_profile.profileAngle(angleError);
                //cout << "Current Angle Accuracy: "<<currAngleAcc <<endl;
                build_profile.build(angleError, &shot);
                build_profile.setLastFrame(src);
                prev = build_profile.getLastFrame();
                //shot suggestion system call this function when we notify end of shot
                imshow("last Frame ",prev);
            }

            white_array.clearArray();
            col_detector.reset();
            col_detector.setShotStartPoint(white_position);
        }

        //! White Positions array
        white_array.addPosition(white_position);
        white_array.drawPath(src);

        //! Draw suggested Point
        shot.drawSuggested(src);

        //! Draw effective collision points
        col_detector.drawPrev(src);

        //! show final image
        imshow("Snooker Player Profile Management", src);

        shot.getUserInput(src);

        if(trigger_val)
        {
            if(!flag)
                //! While ESC is not pressed dont proceed to next shot
                while(waitKey(1) != 27);
            shot.setShotStartP(white_position);
            shot.clear();
            flag = 0;
        }

        //! While ESC is not pressed dont proceed to next shot
        // Uncomment to debug code
//         while(waitKey(1) != 27);

        //! Find colliding points
        col_detector.checkCollision(white_position);

        //! Escape window on pressing 'Q' or 'q'
        code = (char)waitKey(5);
        if( code == 'q' || code == 'Q' )
            break;
    }
    return 0;
}
