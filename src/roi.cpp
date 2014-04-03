#include "roi.hpp"

struct by_y { 
    bool operator()(Vec3f const &a, Vec3f const &b) const { 
        return a[1] < b[1];
    }
};

struct by_x { 
    bool operator()(Vec3f const &a, Vec3f const &b) const { 
        return a[0] < b[0];
    }
};

Roi::Roi()
{
    gotroi=false;
}

Roi::~Roi()
{

}

void Roi::morphOps(Mat &thresh)
{
	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_ELLIPSE,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_ELLIPSE,Size(7,7));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);
	
	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
}

vector<Vec3f>& Roi::getRoi(Mat camerafeed)
{
    Mat HSV;
	//matrix storage for binary threshold image
	Mat threshold;

	//string file=argv[1];

    //Mat camerafeed = imread(file); 

 //   imshow("input", camerafeed);
    cvtColor(camerafeed,HSV,COLOR_BGR2HSV);

    inRange(HSV, Scalar(6,18,72), Scalar(34,70,209), threshold);

    //imshow("filtered", HSV);
    morphOps(threshold);
    //imshow("filtered", threshold);

//    cvtColor( threshold, src_gray, CV_BGR2GRAY );
    
    waitKey(3000);
    vector<Vec3f> circles;

  /// Apply the Hough Transform to find the circles
    HoughCircles( threshold, circles, CV_HOUGH_GRADIENT, 2, 50, 4, 10, 4, 20 );

    /// Draw the circles detected
    sort(circles.begin(),circles.end(),by_x());

    /*for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        printf("\n Ball position : x = %.2f, y= %.2f, Radius = %.2f\n",circles[i][0],circles[i][1],circles[i][2]);
        circle( camerafeed, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // circle outline
        circle( camerafeed, center, radius, Scalar(0,0,255), 3, 8, 0 );
    } */

    sort(circles.begin(),circles.end(),by_x());

    float min1 =circles[0][0]<circles[1][0]?circles[0][0]:circles[1][0];
    circles[0][0] = circles[1][0] = min1;
    float min2 =circles[2][0]<circles[3][0]?circles[2][0]:circles[3][0];
    circles[2][0] = circles[3][0] = min2;
    float max3 =circles[4][0]>circles[5][0]?circles[4][0]:circles[5][0];
    circles[4][0] = circles[5][0] = max3;
    
    sort(circles.begin(),circles.end(),by_y());

    for(int i=0;i<2;++i)
    {
        float min=circles[i*3][1];
        float max=circles[i*3][1];
        for(int j=0;j<3;++j)
        {
            if(i==0){
                if(circles[i*3+j][1]<min)
                    min=circles[i*3+j][1];
            }
            else if(i==1) {
                if(circles[i*3+j][1]>max)
                    max=circles[i*3+j][1];
            }
        }
        for(int j=0;j<3;++j)
            if(i==0)
                circles[i*3+j][1]=min;
            else
                circles[i*3+j][1]=max;
    }

    sort(circles.begin(),circles.end(),by_y());

    final_roi.push_back(circles[1]);
    final_roi.push_back(circles[3]);
    final_roi.push_back(circles[5]);
    final_roi.push_back(circles[4]);
    final_roi.push_back(circles[2]);
    final_roi.push_back(circles[0]);

 /*   printf("\n Final ROI\n\n");
    for( size_t i = 0; i < final_roi.size(); i++ )
        printf("\n x = %.2f, y = %.2f",final_roi[i][0],final_roi[i][1]);*/

    printf("ROI Done ! Press Esc...");
    printf("\n");
    for(size_t i=0;i<final_roi.size()-1;++i)
        line(camerafeed,Point(final_roi[i][0],final_roi[i][1]),Point(final_roi[i+1][0],final_roi[i+1][1]),Scalar(255,0,0),2,CV_AA);
    line(camerafeed,Point(final_roi[final_roi.size()-1][0],final_roi[final_roi.size()-1][1]),Point(final_roi[0][0],final_roi[0][1]),Scalar(255,0,0),2,CV_AA);

    cvWaitKey(0);
    imshow("ROI",camerafeed);

    char keycheck = -1;
    do{
        keycheck=cvWaitKey(10);
    }
    while(keycheck!=27);

    if(keycheck == 27){		// esc key
        cvDestroyAllWindows();
        gotroi=true;
        return final_roi;
    }
}

bool Roi::gotRoi()
{
    return gotroi;
}
