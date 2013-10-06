#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;

// plot points
#define drawCross( center, color, d )								\
	line( img, Point( center.x - d, center.y - d ),					\
		  Point( center.x + d, center.y + d ), color, 2, CV_AA, 0); \
	line( img, Point( center.x + d, center.y - d ),					\
		  Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )
int H_MIN = 84;
int H_MAX = 163;
int S_MIN = 173;
int S_MAX = 256;
int V_MIN = 9;
int V_MAX = 256;
vector<Point> mousev,kalmanv;

void morphOps(Mat &thresh){

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));
	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);
	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
}

Point2f getcenter(Mat &frame)
{

	Mat hsv, threshold;
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Point2f center;
	float radius;
	cvtColor(frame, hsv, CV_BGR2HSV);
	inRange(hsv, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
	morphOps(threshold);
	findContours(threshold, contours, hierarchy, CV_RETR_CCOMP,   CV_CHAIN_APPROX_SIMPLE  );
	minEnclosingCircle((Mat)contours[0], center, radius);
	cout <<radius<<"\t"<<center.x << "\t" << center.y << endl;
	return center;
}

/*
 * Kalman Filter implementation for blue ball
 * Run on test2.mp4
 * ./k1 input/test2.mp4
 */
int main(int argc, char *argv[])
{
	Mat img;
	if(!argv[1])
	{
		cerr << "Video input not given"<< endl;
		cerr << "./k1 input/test3.mp4"<< endl;
		exit(EXIT_FAILURE);
	}
	VideoCapture capture(argv[1]);
	KalmanFilter KF(4, 2, 0);
    Mat_<float> state(4, 1); /* (x, y, Vx, Vy) */
    Mat processNoise(4, 1, CV_32F);
    Mat_<float> measurement(2,1); measurement.setTo(Scalar(0));
    char code = (char)-1;
	namedWindow("snooker");
    for(;;)
    {
		capture >> img;
		Point2f center = getcenter(img);
        KF.statePre.at<float>(0) = center.x;
		KF.statePre.at<float>(1) = center.y;
		KF.statePre.at<float>(2) = 0;
		KF.statePre.at<float>(3) = 0;
		KF.transitionMatrix = *(Mat_<float>(4, 4) << 1,0,0,0,   0,1,0,0,  1,0,1,0,  0,1,0,1);
        setIdentity(KF.measurementMatrix);
        setIdentity(KF.processNoiseCov, Scalar::all(1e-4));
        setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
        setIdentity(KF.errorCovPost, Scalar::all(.1));
		mousev.clear();
		kalmanv.clear();
        for(;;)
        {
//            Point statePt(state(0),state(1));
            Mat prediction = KF.predict();
            Point predictPt(prediction.at<float>(0),prediction.at<float>(1));
			capture >> img;
			center = getcenter(img);
            measurement(0) = center.x;
			measurement(1) = center.y;

			Point measPt(measurement(0),measurement(1));
			mousev.push_back(measPt);
            // generate measurement
            //measurement += KF.measurementMatrix*state;

			Mat estimated = KF.correct(measurement);
			Point statePt(estimated.at<float>(0),estimated.at<float>(1));
			kalmanv.push_back(statePt);

            drawCross( statePt, Scalar(255,255,255), 5 );
            drawCross( measPt, Scalar(0,0,255), 5 );
//            drawCross( predictPt, Scalar(0,255,0), 3 );
//			line( img, statePt, measPt, Scalar(0,0,255), 3, CV_AA, 0 );
//			line( img, statePt, predictPt, Scalar(0,255,255), 3, CV_AA, 0 );

			for (int i = 0; i < mousev.size()-1; i++) {
				line(img, mousev[i], mousev[i+1], Scalar(255,255,0), 1);
			}
			for (int i = 0; i < kalmanv.size()-1; i++) {
				line(img, kalmanv[i], kalmanv[i+1], Scalar(0,255,0), 1);
			}
            imshow( "snooker", img );
            code = (char)waitKey(100);
            if( code > 0 )
                break;
        }
        if( code == 27 || code == 'q' || code == 'Q' )
            break;
    }
	return 0;
}
