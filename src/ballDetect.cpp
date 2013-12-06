#include <iostream>
#include "ballDetect.h"
#include <string>

using namespace std;
using namespace cv;

String ballTag[] = {"Red","White","Black","Blue","Yellow","Pink", "Brown", "Green"};
int shot=-1,flag=0, shotTemp=0;
int xPrev=0, yPrev=0;

ballDetect :: ballDetect(){
    minval = new Scalar[8];
    maxval = new Scalar[8];

    //minimum threshold values
    minval[0] = Scalar(174,105,121); //red
    minval[1] = Scalar(0,0,200); //white
    minval[2] = Scalar(0,0,0); //black
    minval[3] = Scalar(70,48,20); //blue
    minval[4] = Scalar(23,125,176); //yellow
    minval[5] = Scalar(0,60,235); //pink
    minval[6] = Scalar(17,155,165); //brown
    minval[7] = Scalar(67,238,112); //green

    //maximum threshold values
    maxval[0] = Scalar(255,255,255); //red
    maxval[1] = Scalar(255,70,255); //white
    maxval[2] = Scalar(255,255,13); //black
    maxval[3] = Scalar(163,255,255); //blue
    maxval[4] = Scalar(47,255,255); //yellow
    maxval[5] = Scalar(9,186,255); //pink
    maxval[6] = Scalar(26,255,255); //brown
    maxval[7] = Scalar(82,255,223); //green
}

string ballDetect :: intToString(int number){
    stringstream ss;
    ss << number;
    return ss.str();
}

void ballDetect ::  drawObject(int x, int y,Mat &frame,int ballIndex,int redIndex){

    //use some of the openCV drawing functions to draw crosshairs on your tracked image!

    //added 'if' and 'else' statements to prevent
    //memory errors from writing off the screen (ie. (-25,-25) is not within the window!)

    circle(frame,Point(x,y),10,Scalar(0,255,0),2);

    // putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);
    putText(frame,"Shot No. "+intToString(shot+1),Point(90,40),1,1,Scalar(255,0,0),2);

    if(!ballIndex)
        putText(frame,ballTag[ballIndex]+intToString(redIndex),Point(x,y-5),1,1,Scalar(255,255,0),1);
    else if(ballIndex==1){
        putText(frame,ballTag[ballIndex],Point(x,y-5),1,1,Scalar(255,255,0),1);
        if(!xPrev)
            xPrev=x;
        if(!yPrev)
            yPrev=y;
        if(std::abs(x-xPrev)==0 && std::abs(y-yPrev)==0)
        {
            if(!flag)
                shot++;
            flag=1;
        }
        else{
            shotTemp++;
            if(shotTemp>20){
                shotTemp=0;
                flag=0;
            }   
        }
        xPrev=x;
        yPrev=y;
    }
    else{
        putText(frame,ballTag[ballIndex],Point(x,y-5),1,1,Scalar(255,255,0),1);
    }
}

void ballDetect :: morphOps(Mat &processed){

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle

    Mat erodeElement = getStructuringElement( MORPH_ELLIPSE,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement( MORPH_ELLIPSE,Size(7,7));

    erode(processed,processed,erodeElement);
    erode(processed,processed,erodeElement);

    dilate(processed,processed,dilateElement);
    dilate(processed,processed,dilateElement);
}

void ballDetect :: trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed,int ballIndex){

    Mat temp;
    threshold.copyTo(temp);

    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
    //use moments method to find our filtered object
    double refArea = 0;
    bool objectFound = false;
    if (hierarchy.size() > 0) {
        int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
            for (int index = 0; index >= 0; index = hierarchy[index][0]) {

                Moments moment = moments((cv::Mat)contours[index]);
                double area = moment.m00;

                //if the area is less than 20 px by 20px then it is probably just noise
                //if the area is the same as the 3/2 of the image size, probably just a bad filter
                //we only want the object with the largest area so we safe a reference area each
                //iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
                    x = moment.m10/area;
                    y = moment.m01/area;
                    objectFound = true;

                }else objectFound = false;
            }
            //let user know you found an object
            if(objectFound ==true){
                //putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
                //printf("\n Object at : x=%d y=%d",x,y);
                //draw object location on screen
                drawObject(x,y,cameraFeed,ballIndex,0);}

        }else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
    }
}

void ballDetect :: initDetect(char *videoInput){

    VideoCapture capture;
    Mat src,src_HSV,processed;
    int x=0;
    int y=0;

    capture.open(videoInput);
    capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
    
    while(1){
        //store image to matrix
        capture.read(src);
        //convert frame from BGR to HSV colorspace
        cvtColor(src,src_HSV,COLOR_BGR2HSV);

        for(int i=0;i<8;++i)
        {
            inRange(src_HSV,minval[i],maxval[i],processed);
            morphOps(processed);
            if(i!=0)
                trackFilteredObject(x,y,processed,src,i);
            else{
                vector<Vec3f> circles;
                // HoughCircles(processed,circles, CV_HOUGH_GRADIENT,1,src_HSV.rows/128,20,12,5,40);
                HoughCircles(processed,circles, CV_HOUGH_GRADIENT,1,8,20,12,5,40);
                for( size_t i = 0; i < circles.size(); i++ ) {
                    Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                    //int radius = cvRound(circles[i][2]);
                    drawObject(cvRound(circles[i][0]), cvRound(circles[i][1]), src,0,(int)i);
                    // printf("\n Object at : x=%d y=%d",cvRound(circles[i][0]),cvRound(circles[i][1]));
                }
            }
            imshow("source",src);
        }
        waitKey(5);
    }
}
