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
#include "managelogin.hpp"
#include "collisionDetector.hpp"
#include "sclassifier.hpp"
#include "roi.hpp"

#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    VideoCapture capture;		//! Videocapture to capture video object
    Mat src,prev;					//! Matrix object to get input
    Mat original;
    Mat previous;
    bool visited = false;
    Point white_position(-1, -1); //! White Ball Position
    DetectBall white_detector;
    char code = (char)-1;
    unsigned int shottype=-1;
    Vector<Vec3f> final_roi;
    Point final;
    Point normalEndpoint;
    int whiteSize = 9999;

    SnKalman kfchecker;
    ManageLogin login;
    DetectShot shot_detector;
    ShotArray white_array;
    CollisionDetector col_detector;
    Shot shot;
    Sclassifier shot_classify;
    Roi roi;

    int flag = 1;
    static string userId;
    do{
        userId = login.getUserID();
    }while(userId == "\0");

    BuildProfile build_profile(userId);

    /*!
     * Open user input video from given path
     * and set frame width & height.
     */
    capture.open(argv[1]);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
    previous = capture.read(src);

    do
    {
        //! store image to matrix
        int c = capture.read(src);
        if(!c)
            exit(0);

        original = src.clone();
        // if(!roi.gotRoi()) {
        //     final_roi = roi.getRoi(src);
        //     printf("\n");
        // }

        //! detect white ball
        white_position = white_detector.detectWhite(src);

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

                shottype = shot.shottype; 

                double angleError = shot.showFeedback(src, &white_array,shot_classify.getShotString(shottype));

                //int currAngleAcc = build_profile.profileAngle(angleError);
                //cout << "Current Angle Accuracy: "<<currAngleAcc <<endl;
                build_profile.addCurrent(userId,angleError,
					 white_array.totalDist(),
					 shot.getSuggDist(),
					 white_array.totalTime(),
					 white_array.shotVelocity()*(0.367347),
					 shottype);
                build_profile.setLastFrame(src);
                prev = build_profile.getLastFrame();
                //shot type 

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
        // imshow("Snooker Player Profile Management", src);

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
        imshow("Snooker Player Profile Management", src);

        //! Find colliding points
        col_detector.checkCollision(white_position, previous, original, white_array, final, normalEndpoint, whiteSize);
        if(white_array.white_positions.size() > (whiteSize+9) && !visited){

            // cout <<  white_array.white_positions[(whiteSize+2)] << endl;
            shot.shotType(final, normalEndpoint, white_array.white_positions[(whiteSize+1)]);
            visited = true;
            Mat angle;
            angle = src;
            line(angle, final, normalEndpoint, Scalar(100, 255, 0), 2, CV_AA);
            line(angle, normalEndpoint,  white_array.white_positions[(whiteSize+1)], Scalar(100, 255, 0), 2, CV_AA);
            line(angle, final,  white_array.white_positions[(whiteSize+1)], Scalar(100, 255, 0), 2, CV_AA);
            imshow("angle",angle);
        }

        //! Escape window on pressing 'Q' or 'q
        code = (char)waitKey(5);
        if( code == 'q' || code == 'Q' )
            break;

        previous = original.clone();

        // Uncomment to debug code
        // while(waitKey(1) != 27);

    }while(waitKey(1) != 27);

    return 0;
}
