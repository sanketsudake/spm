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
#include "gui.hpp"

#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480

using namespace std;
using namespace cv;

//Green part co-ordinates
Point left_top;
Point right_bottom;

void getGreenMat(Mat );

int main(int argc, char **argv)
{
    VideoCapture capture;		//! Videocapture to capture video object
    Mat src,prev;					//! Matrix object to get input
    Mat original;
    Mat previous;
    bool visited = false;
    Mat profile_picture;
    Point white_position(-1, -1); //! White Ball Position
    char code = (char)-1;
    unsigned int shottype=-1;
    Vector<Vec3f> final_roi;
    Point final;
    Point normalEndpoint;
    int whiteSize = 0;
    // Rect roi;

    SnKalman kfchecker;
    DetectBall white_detector;
    ManageLogin login;
    DetectShot shot_detector;
    ShotArray white_array;
    CollisionDetector col_detector;
    Shot shot;
    Sclassifier shot_classify;
    Roi roi;
    // BuildProfile build_profile;
    GUI interface;
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

    // getGreenMat(src);

    do
    {
        // ! store image to matrix
        int c = capture.read(src);
        if(!c)
            exit(0);

        // roi = Rect(left_top.x, left_top.y, right_bottom.x-left_top.x, right_bottom.y-left_top.y);
        // src = src(roi);

        original = src.clone();
        // if(!roi.gotRoi()) {
        //     final_roi = roi.getRoi(src);
        //     printf("\n");
        // }
        // cout<<"Rows: "<<original.rows<<endl;
        // cout<<"Cols: "<<original.cols<<endl;

        //! detect white ball
        white_position = white_detector.detectWhite(src);

        //! Correct position through kalman filter
        white_position = kfchecker.correctPoisition(white_position),
                       white_detector.mapPosition(src, white_position, 1);

        //! Display frame number
        interface.frame_count = white_detector.showFrameNo();

        //!Call shot checker for every frame
        int trigger_val = shot_detector.shotChecker(src, &interface.shot_count, white_position);
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

                shottype = shot_classify.shot_classifier(0.0,-1,1,0,&shot);


                double angleError = shot.showFeedback(src, &white_array,shot_classify.getShotString(shottype));

                //int currAngleAcc = build_profile.profileAngle(angleError);
                //cout << "Current Angle Accuracy: "<<currAngleAcc <<endl;
                build_profile.build(angleError, &shot);
                build_profile.addCurrent(userId,angleError, white_array.totalDist(), white_array.totalTime(),
                        white_array.shotVelocity()*(0.367347));
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

        //! Find colliding points
        col_detector.checkCollision(white_position, previous, original, white_array, final, normalEndpoint, whiteSize);
        
        if(white_array.white_positions.size() > (whiteSize+2) && !visited){
            // cout <<  white_array.white_positions[(whiteSize+2)] << endl;
            shot.shotType(final, normalEndpoint, white_array.white_positions[(whiteSize+1)]);
            visited = true;
            // Mat angle;
            // angle = src;
            // line(angle, final, normalEndpoint, Scalar(100, 255, 0), 2, CV_AA);
            // line(angle, normalEndpoint,  white_array.white_positions[(whiteSize+1)], Scalar(100, 255, 0), 2, CV_AA);
            // line(angle, final,  white_array.white_positions[(whiteSize+1)], Scalar(100, 255, 0), 2, CV_AA);
            // imshow("angle",angle);
        }

        //! Escape window on pressing 'Q' or 'q
        code = (char)waitKey(5);
        if( code == 'q' || code == 'Q' )
            break;

        previous = original.clone();

        // Uncomment to debug code
        // while(waitKey(1) != 27);
        
        interface.ShowProfilePicture();
        interface.ShowGameStatistics(userId);
        interface.ShowProfileTable();

        imshow("Snooker Player Profile Management", src);
        moveWindow("Snooker Player Profile Management", (SCREEN_WIDTH-original.cols), 0);

    }while(waitKey(1) != 27);

    return 0;
}

void getGreenMat(Mat frame)
{
    Mat gray;
    Mat temp;
    Rect roi;
    vector<Vec3f> pockets; 

    roi = Rect(0,0,100,100);
    temp = frame(roi);
    cvtColor(temp, gray, CV_BGR2GRAY);
    HoughCircles(gray, pockets, CV_HOUGH_GRADIENT, 1, gray.rows/16, 50, 15, 5, 10);
    // cout<<"Left: "<<pockets.size();
    for( size_t i = 0; i < pockets.size(); i++ ) {
        Point center(cvRound(pockets[i][0]), cvRound(pockets[i][1]));
        int radius = cvRound(pockets[i][2]);
        // circle(frame, center, 1, Scalar(0, 255, 255), 2);
        left_top = center;
    }

    roi = Rect(frame.cols-100,frame.rows-100,100,100);
    temp = frame(roi);
    cvtColor(temp, gray, CV_BGR2GRAY);
    HoughCircles(gray, pockets, CV_HOUGH_GRADIENT, 1, gray.rows/16, 50, 15, 5, 10);
    // cout<<"Right: "<<pockets.size();
    for( size_t i = 0; i < pockets.size(); i++ ) {
        Point center(cvRound(pockets[i][0]), cvRound(pockets[i][1]));
        int radius = cvRound(pockets[i][2]);
        // circle(temp, center, 1, Scalar(0, 255, 255), 2);
        right_bottom = center;  
    }

    right_bottom.x += frame.cols-100;
    right_bottom.y += frame.rows-100;

    // circle(frame, left_top, 1, Scalar(0, 255, 255), 2);
    // circle(frame, right_bottom, 1, Scalar(0, 255, 255), 2);
    // imshow("Sample", frame);
}
