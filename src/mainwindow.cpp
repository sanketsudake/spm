#include "../include/mainwindow.h"
#include "../build/ui_mainwindow.h"

#include "../include/detectball.hpp"
#include "../include/detectshot.hpp"
#include "../include/buildprofile.hpp"
#include "../include/managelogin.hpp"
#include "../include/shot.hpp"
#include "../include/shotArray.hpp"
#include "../include/managelogin.hpp"
#include "../include/collisionDetector.hpp"
#include "../include/sclassifier.hpp"
#include "../include/roi.hpp"
#include "../include/gui.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QPixmap>
#include <QString>
#include <QTimer>

#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480

using namespace std;
using namespace cv;

Mat roi_image = Mat(Size(200, 200), CV_32F, CV_RGB(255,255,255));

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer::singleShot(1000, this, SLOT(showFullScreen()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openVideo(char *video)
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

    // do{
    //     userId = login.getUserID();
    // }while(userId == "\0");

    BuildProfile build_profile((string)"ronnie11");

    /*!
     * Open user input video from given path
     * and set frame width & height.
     */

    capture.open((char *)video);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

    do{
        int c = capture.read(src);
        if (!c)
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
                interface.cut = 88;
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
                // imshow("last Frame ",prev);
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

        QImage roiImage = QImage((const unsigned char*)(roi_image.data), roi_image.cols,roi_image.rows,QImage::Format_RGB888).rgbSwapped();
        ui->label_roi->setPixmap(QPixmap::fromImage(roiImage));


        if(white_array.white_positions.size() > (whiteSize+9) && !visited){
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
        
        // interface.ShowProfilePicture();
        // interface.ShowGameStatistics(userId);
        // interface.ShowProfileTable();
        //
        imshow("Snooker Player Profile Management", src);
        // moveWindow("Snooker Player Profile Management", (SCREEN_WIDTH-original.cols), 0);

        // cv::resize(src, src, Size(src.cols*0.75, src.rows*0.75), 2, 2, INTER_CUBIC);

        showProfile(interface);
        QImage imageView = QImage((const unsigned char*)(src.data), src.cols,src.rows,QImage::Format_RGB888).rgbSwapped();
        ui->label->setPixmap(QPixmap::fromImage(imageView));

    }while(waitKey(1)!=27);
}

void MainWindow::showProfile(GUI interface){

    QTableWidgetItem *item = new QTableWidgetItem();
    item->text(QString::number(interface.straight));

    ui->tableWidget_9->setItem(0, 0, item);
    // ui->tableWidget_9->setItem(0, 0, new QTableWidgetItem(QString::number(interface.straight)));
    ui->tableWidget_9->setItem(0, 1, new QTableWidgetItem(QString::number(interface.cut)));
    ui->tableWidget_9->setItem(0, 2, new QTableWidgetItem(QString::number(interface.safety)));

    ui->tableWidget_12->setItem(0, 0, new QTableWidgetItem(QString::number(interface.spin)));
    ui->tableWidget_12->setItem(0, 1, new QTableWidgetItem(QString::number(interface.power)));
    ui->tableWidget_12->setItem(0, 3, new QTableWidgetItem(QString::number(interface.overall)));
}
