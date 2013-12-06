#include <iostream>
#include "ballDetect.h"
#include <unistd.h>
#include <string>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<string>
#include <cmath>
#include <ctime>
using namespace std;
using namespace cv;

// Ball tags for showing on the screen
String ballTag[] = {"Red", "White", "Black", "Blue",
					"Yellow", "Pink", "Brown", "Green"};
//
int shot = -1,flag = 0, shotTemp = 0, shot_change_trigger = 0;
int xPrev = 0, yPrev = 0;
double white_velocity = 0;
time_t shot_start = time(NULL), shot_end = time(NULL);
vector<Point> white_positions;

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
	putText(frame,"Shot No. "+intToString(shot+1),Point(90,40),1,1,Scalar(255,255,0),2);
    // putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);

	// Hard coded shot error calculations
	Point start(258, 268), actual(805, 290), expected(805 - 15, 290 + 16);
	Scalar expected_color(50, 220, 50), actual_color(0, 50, 200), ball_col(180, 50, 50);
	// Circle start, actual , expected
	circle(frame, start, 10, ball_col, 2);
	circle(frame, expected, 10, ball_col, 2);
	line(frame, start, expected, expected_color, 1, CV_AA, 0);
    putText(frame, "Expec. shot -----", Point(90,80), 1, 1, expected_color ,2);
	putText(frame, "Actual shot ", Point(90,120), 1, 1, actual_color ,2);
	putText(frame, "-----", Point(200,120), 1, 1, Scalar(255, 255, 255) ,2);

	// slope calculation for hard-coded shot
	double slope1 = 0, slope2 = 0, angle1 = 0, angle2 = 0, error = 0;
	slope1 = (double)(expected.y - start.y) / (expected.x - start.x);
	slope2 = (double)(actual.y - start.y) / (actual.x - start.x);
	angle1 = atan(slope1) * 180 / M_PI;
	angle2 = atan(slope2) * 180 / M_PI;
	error = std::abs(angle1 - angle2);


    if(!ballIndex)
        putText(frame,ballTag[ballIndex]+intToString(redIndex),
				Point(x,y-5),1,1,Scalar(255,255,0),1);
    else if(ballIndex == 1){
		// std::cout <<  "Points" << x << " " << y << " " <<  (double)(y - yPrev)/(x - xPrev)<< endl;
		white_positions.push_back(Point(x, y));
        putText(frame,ballTag[ballIndex],Point(x,y-5),1,1,Scalar(255,255,0),1);
		int white_positions_size = (int)white_positions.size();
		for(int i = 0; i < white_positions_size - 1; i++ )
		{
			line(frame, white_positions[i], white_positions[i+1], Scalar(255, 255, 255), 1, CV_AA, 0);
			//circle(frame,white_positions[i],1,Scalar(0,0 ,255),2);
		}
		if(shot)
		{
			putText(frame, "White Velo. " + intToString((int)white_velocity) + "."
					+ intToString((int)(white_velocity * 1000) - ((int)white_velocity) * 1000)
					+ " pixels/sec", Point(500, 550), 1, 1, Scalar(20, 25, 50) , 2, 1);
			if(shot < 2)
				putText(frame, "ERROR : " + intToString((int)error) + "."
						+ intToString((int)(error * 1000) - ((int)error) * 1000)
						+ " deg", Point(500, 500), 1, 1, Scalar(50, 155, 150), 2, 1);
			circle(frame, actual, 10, ball_col, 2);
			line(frame, start, actual, actual_color, 1, CV_AA, 0);
		}
        if(!xPrev)
            xPrev=x;
        if(!yPrev)
            yPrev=y;
        if(std::abs(x-xPrev)==0 && std::abs(y-yPrev)==0)
        {
            if(!flag)
			{
				shot++;
				shot_end = time(NULL);
                // Calculating velocity of white balls
				// First finding total distance covered
				double white_distance = 0, time_diff = 0;
				int white_positions_size = (int)white_positions.size();
				double x_distance = 0, y_distance = 0, curr_distance = 0;
				for(int i = 0; i < white_positions_size - 1; i++ )
				{
					x_distance = white_positions[i+ 1].x - white_positions[i].x;
					y_distance = white_positions[i+ 1].y - white_positions[i].y;
					curr_distance = sqrt(pow(x_distance, 2) + pow(y_distance, 2));
					white_distance = white_distance + curr_distance;
				}
				time_diff = difftime(shot_end, shot_start);
				white_velocity = white_distance / time_diff;
				shot_start = time(NULL);
				white_positions.clear();
				shot_change_trigger = 1;
			}
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
	// if(shot < 2){
	// 	if(x != xPrev)
	// 	{
	// 		double white_slope = (double)(y - yPrev) / (x - xPrev );
	// 		//std::cout <<  x << " "<< y << " "<< xPrev << " "<< yPrev <<  " " << white_slope << endl; //
	// 		//std::cout <<  y - yPrev << " " << x - xPrev << " " << white_slope << endl;
	// 	}
	// 	else
	// 	{
	// 		//std::cout <<  x << " "<< y << " "<< xPrev << " "<< yPrev <<  " " << "Nil" << endl;
	// 		//std::cout <<  y - yPrev << " " << x - xPrev << " " << "Nil" << endl;
	// 	}
	// }
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
            // else{
            //     vector<Vec3f> circles;
            //     // HoughCircles(processed,circles, CV_HOUGH_GRADIENT,1,src_HSV.rows/128,20,12,5,40);
            //     HoughCircles(processed,circles, CV_HOUGH_GRADIENT,1,8,20,12,5,40);
            //     for( size_t i = 0; i < circles.size(); i++ )
			// 	{
            //         Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            //         //int radius = cvRound(circles[i][2]);
            //         drawObject(cvRound(circles[i][0]), cvRound(circles[i][1]), src,0,(int)i);
            //         // printf("\n Object at : x=%d y=%d",cvRound(circles[i][0]),cvRound(circles[i][1]));
            //     }
            //
			imshow("source",src);
			// Change value of shot trigger
			// After showing frame
			if(shot_change_trigger)
				shot_change_trigger = 0;
		}
        waitKey(5);
    }
}
