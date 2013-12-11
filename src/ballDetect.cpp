#include <iostream>
#include "ballDetect.h"
#include <unistd.h>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdio>

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

// specify expected shot - point
Point p1(-1, -1);
Point p2(-1, -1);
Point p3(-1, -1);

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	//namedWindow("test");
	Mat img = *((Mat *)userdata);
	if  ( event == EVENT_LBUTTONDOWN )
	{
		if((x >= 38+6 && x <= 1014-6) && (y >= 35+6 && y <= 541-6))
		{
			if(p1.x == -1 && p1.y == -1)
			{
				p1.x = x;
				p1.y = y;
			}
			else  if(p2.x == -1 && p2.y == -1)
			{
				line(img, p1, Point(x,y), Scalar(50, 220, 50), 1, CV_AA);
				p2 = Point(x, y);
			}
			else
			{
				line(img, p2, Point(x,y), Scalar(50, 220, 50), 1, CV_AA);
				p3=Point(x,y);
				setMouseCallback("source", NULL,NULL);
			}
			// cout << "Left button of the mouse is clicked - position ("
			// 	  << x << ", " << y << ")" << endl;
			circle(img,Point(x,y),10,Scalar(0,255,0));
			//imshow("source",img);
		}
	}
	else if  ( event == EVENT_RBUTTONDOWN )
	{
		//cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		setMouseCallback("source", NULL, NULL);
	}
	else if  ( event == EVENT_MBUTTONDOWN )
	{
		//cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if ( event == EVENT_MOUSEMOVE )
	{
		//cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
	}
}

ballDetect :: ballDetect()
{
    minval = new Scalar[8];
    maxval = new Scalar[8];

    // minimum threshold values
    minval[0] = Scalar(174, 105, 121); //red
    minval[1] = Scalar(0, 0, 200); //white
    minval[2] = Scalar(0, 0, 0); //black
    minval[3] = Scalar(70, 48, 20); //blue
    minval[4] = Scalar(23, 125, 176); //yellow
    minval[5] = Scalar(0, 60, 235); //pink
    minval[6] = Scalar(17, 155, 165); //brown
    minval[7] = Scalar(67, 238, 112); //green

    // maximum threshold values
    maxval[0] = Scalar(255, 255, 255); //red
    maxval[1] = Scalar(255, 70, 255); //white
    maxval[2] = Scalar(255, 255, 13); //black
    maxval[3] = Scalar(163, 255, 255); //blue
    maxval[4] = Scalar(47, 255, 255); //yellow
    maxval[5] = Scalar(9, 186, 255); //pink
    maxval[6] = Scalar(26, 255, 255); //brown
    maxval[7] = Scalar(82, 255, 223); //green
}

string ballDetect :: intToString(int number)
{
    stringstream ss;
    ss << number;
    return ss.str();
}

void ballDetect :: shotTrigger(Mat &frame)
{
	Scalar expected_color(50, 220, 50), actual_color(0, 50, 200),
		ball_col(180, 50, 50);
	if(shot)
	{
		// Display white velocity
		putText(frame, "White Velo. "
				+ intToString((int)white_velocity) + "."
				+ intToString((int)(white_velocity * 1000)
							  - ((int)white_velocity) * 1000)
				+ " cm/sec", Point(500, 550), 1, 1, Scalar(20, 25, 50), 2, 1);

		// Hard coded shot error calculations
		// Circle start, actual , expected
		Point start(258, 268), actual(805, 290), expected(805 - 15, 290 + 16);
		// Hard-coded code to be removed after implementation of first collision
		// Detection
		// circle(frame, start, 10, ball_col, 2);
		// circle(frame, expected, 10, ball_col, 2);
		// line(frame, start, expected, expected_color, 1, CV_AA, 0);

		// slope calculation for hard-coded shot
		double slope1 = 0, slope2 = 0, angle1 = 0, angle2 = 0, error = 0;
		slope1 = (double)(expected.y - start.y) / (expected.x - start.x);
		slope2 = (double)(actual.y - start.y) / (actual.x - start.x);
		// tan inverse of both slopes and find difference
		angle1 = atan(slope1) * 180 / M_PI;
		angle2 = atan(slope2) * 180 / M_PI;
		error = std::abs(angle1 - angle2);

		// Display error between suggested and actual shot in terms of degree
		// Currently just showing for first shot
		putText(frame, "ERROR : " + intToString((int)error) + "."
				+ intToString((int)(error * 1000) - ((int)error) * 1000)
				+ " deg", Point(500, 500), 1, 1, Scalar(50, 155, 150),
				2, 1);
		// Highlight hit point hard -coded
		circle(frame, actual, 10, ball_col, 2);
		// Draw line between initial and hard-coded hit point
		line(frame, start, actual, actual_color, 1, CV_AA, 0);
	}
	// Accepting user input for expected shot phenomenon
    // specify expected shot
	//p1=Point(-1, -1);
	p2=Point(-1, -1);
	p3=Point(-1, -1);

	setMouseCallback("source", CallBackFunc, &frame);
	while (p3.x == -1 && p3.y == -1)
	{
		// ESC to see shot
		putText(frame, "Specify shot and Press ESC.", Point(200, 40), 1, 1,
				Scalar(255,255,0),2);
		imshow("source", frame);
		cvWaitKey(10);
	}
	if(p2.x>0 && p2.y>0)
	{
		line(frame, p1, p2, Scalar(50, 220, 50), 1, CV_AA);
	}
	else if(p3.x>0 && p3.y>0)
	{
		line(frame, p2, p3, Scalar(50, 220, 50), 1, CV_AA);
		setMouseCallback("source",NULL,NULL);
	}
}

void ballDetect :: drawObject(int x, int y, Mat &frame, int ballIndex,
							  int redIndex)
{
    // use some of the openCV drawing functions to draw crosshairs on your
	// tracked image! added 'if' and 'else' statements to prevent
    // memory errors from writing off the screen (ie. (-25,-25) is not within
	// the window!)
	circle(frame, Point(x,y), 10, Scalar(0,255,0), 2);

	// Show ball positions near ball
	// putText(frame, intToString(x) + "," + intToString(y), Point(x, y+30),
	// 		1, 1, Scalar(0,255,0),2);

	// Show shot no on top left corner
	putText(frame, "Shot No. " + intToString(shot+1), Point(90,40), 1, 1,
			Scalar(255,255,0),2);
	Scalar expected_color(50, 220, 50), actual_color(0, 50, 200),
		ball_col(180, 50, 50);
    putText(frame, "Expec. shot -----", Point(90,80), 1, 1, expected_color ,2);
	putText(frame, "Actual shot ", Point(90,120), 1, 1, actual_color, 2);
	putText(frame, "-----", Point(200,120), 1, 1, Scalar(255, 255, 255) ,2);

    if(!ballIndex)
        putText(frame,ballTag[ballIndex] + intToString(redIndex),
				Point(x,y-5), 1, 1, Scalar(255,255,0), 1);
    else if(ballIndex == 1)
	{
        putText(frame, ballTag[ballIndex], Point(x,y-5), 1, 1,
				Scalar(255,255,0), 1);

		white_positions.push_back(Point(x, y));
		// Highlight white ball path
		int white_positions_size = (int)white_positions.size();
		for(int i = 0; i < white_positions_size - 1; i++ )
		{
			line(frame, white_positions[i], white_positions[i+1],
				 Scalar(255, 255, 255), 1, CV_AA, 0);
			circle(frame, white_positions[i], 1, Scalar(255, 255 ,200), 2);
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
				p1.x = x;
				p1.y = y;
				shotTrigger(frame);
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
				white_velocity = (white_distance / time_diff) * (0.367347);
				shot_start = time(NULL);
				white_positions.clear();
				shot_change_trigger = 1;
			}
			flag=1;
        }
        else
		{
            shotTemp++;
            if(shotTemp>20)
			{
				shotTemp=0;
                flag=0;
            }
        }
        xPrev=x;
        yPrev=y;
    }
    else
        putText(frame, ballTag[ballIndex], Point(x,y-5), 1, 1, Scalar(255, 255, 0), 1);

	// Currently unwanted code
	// Used for checking slope values of white positions
	// Whether they remain constant or not
	// Seen a lot of variations
	// if(shot < 2)
	// {
	// 	if(x != xPrev)
	// 	{
	// 		double white_slope = (double)(y - yPrev) / (x - xPrev );
	// 		std::cout <<  x << " "<< y << " "<< xPrev << " "<< yPrev <<
	// 			" " << white_slope << endl; //
	// 		std::cout <<  y - yPrev << " " << x - xPrev <<
	// 			" " << white_slope << endl;
	// 	}
	// 	else
	// 	{
	// 		std::cout <<  x << " "<< y << " "<< xPrev << " "<< yPrev <<
	// 			" " << "Nil" << endl;
	// 		std::cout <<  y - yPrev << " " << x - xPrev << " "
	// 				  << "Nil" << endl;
	// 	}
	// }
}

void ballDetect :: morphOps(Mat &processed)
{

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle

    Mat erodeElement = getStructuringElement( MORPH_ELLIPSE, Size(3,3));
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement( MORPH_ELLIPSE, Size(7,7));

    erode(processed, processed, erodeElement);
    erode(processed, processed, erodeElement);

    dilate(processed, processed, dilateElement);
    dilate(processed, processed, dilateElement);
}

void ballDetect :: trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed,int ballIndex){

    Mat temp;
    threshold.copyTo(temp);

    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    //use moments method to find our filtered object
    double refArea = 0;
    bool objectFound = false;
    if (hierarchy.size() > 0)
	{
        int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects < MAX_NUM_OBJECTS)
		{
            for (int index = 0; index >= 0; index = hierarchy[index][0])
			{
                Moments moment = moments((cv::Mat)contours[index]);
                double area = moment.m00;

                // if the area is less than 20 px by 20px then it is probably
				// just noise if the area is the same as the 3/2 of the image size,
				// probably just a bad filter we only want the object with the
				// largest area so we safe a reference area each iteration and
				// compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea)
				{
                    x = moment.m10/area;
                    y = moment.m01/area;
                    objectFound = true;

                }
				else
					objectFound = false;
            }
            //let user know you found an object
            if(objectFound == true)
			{
                //putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
                //printf("\n Object at : x=%d y=%d",x,y);
                //draw object location on screen
                drawObject(x, y, cameraFeed, ballIndex, 0);
			}
		}
		else
		{
			putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER",
					Point(0,50), 1, 2, Scalar(0,0,255),2);
		}
    }
}


void ballDetect :: initDetect(char *videoInput)
{
    VideoCapture capture;
    Mat src, src_HSV, processed;
    int x=0;
    int y=0;

	capture.open(videoInput);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

    while(1)
	{
        //store image to matrix
        capture.read(src);
        //convert frame from BGR to HSV colorspace
        cvtColor(src, src_HSV, COLOR_BGR2HSV);

        for(int i=0; i<8; ++i)
        {
            inRange(src_HSV, minval[i], maxval[i], processed);
            morphOps(processed);
            if(i != 0)
                trackFilteredObject(x, y, processed, src, i);
			if(p3.x>0 && p3.y>0)
			{
				line(src,p1,p2,Scalar(50, 220, 50), 1, CV_AA);
				line(src,p2,p3,Scalar(50, 220, 50), 1, CV_AA);
			}
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
			imshow("source", src);
			// Change value of shot trigger
			// After showing frame
			if(shot_change_trigger)
			{
				shot_change_trigger = 0;
				// While ESC is not pressed dont proceed to next shot
				while(waitKey(1) != 27);
			}
		}
        waitKey(5);
    }
}
