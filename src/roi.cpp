#include "../include/roi.hpp"

struct by_y { 
    bool operator()(Vec3f const &a, Vec3f const &b) const { 
        if(a[1] < b[1])
            return (a[1] < b[1]);
//        else
//            return (a[0] < b[0]);
    }
};

struct by_x { 
    bool operator()(Vec3f const &a, Vec3f const &b) const {
        if(a[0] <= b[0])
            return (a[0] < b[0]);
//        else
//            return (a[1] < b[1]);
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

	Mat erodeElement = getStructuringElement( MORPH_ELLIPSE,Size(3,3));
	Mat dilateElement = getStructuringElement( MORPH_ELLIPSE,Size(7,7));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);
	
	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
}

vector<Vec3f>& Roi::getRoi(Mat &camerafeed)
{
    Mat HSV;
	Mat threshold;
    cvtColor(camerafeed,HSV,COLOR_BGR2HSV);

    inRange(HSV, Scalar(6,18,72), Scalar(34,70,209), threshold);
    morphOps(threshold);
    //waitKey(3000);
    vector<Vec3f> circles,min_2x,min3y,min3y_2;

    HoughCircles( threshold, circles, CV_HOUGH_GRADIENT, 2, 50, 4, 10, 8, 20 );

    sort(circles.begin(),circles.end(),by_y());
   
    min3y.push_back(circles[0]); 
    min3y.push_back(circles[1]); 
    min3y.push_back(circles[2]); 
   
    min3y_2.push_back(circles[3]);
    min3y_2.push_back(circles[4]);
    min3y_2.push_back(circles[5]);
    
    float miny_new = min3y[0][1];

    for(int i=0;i<min3y.size();++i) 
        if(miny_new > min3y[i][1])
            miny_new = min3y[i][1];
    

    min3y[0][1] = min3y[1][1] = min3y[2][1] = miny_new;

    sort(min3y.begin(),min3y.end(),by_x());
    
    // done first 3 y
    
    float miny_new_2 = min3y_2[0][1];

    for(int i=0;i<min3y_2.size();++i) 
        if(miny_new_2 < min3y_2[i][1])
            miny_new_2 = min3y_2[i][1];

    min3y_2[0][1] = min3y_2[1][1] = min3y_2[2][1] = miny_new_2;
    
    sort(min3y_2.begin(),min3y_2.end(),by_x());
    
    min3y[0][0] = min3y_2[0][0] = (min3y[0][0]<min3y_2[0][0]?min3y[0][0]:min3y_2[0][0]);
    min3y[2][0] = min3y_2[2][0] = (min3y[2][0]>min3y_2[2][0]?min3y[2][0]:min3y_2[2][0]);

    for(int i=0;i<min3y.size();++i)
        final_roi.push_back(min3y[i]);

    for(int i=min3y_2.size()-1;i>=0;--i)
        final_roi.push_back(min3y_2[i]);
   
    for(size_t i=0;i<final_roi.size();++i)
        printf("\n Final ROI : x = %.2f, \ty = %.2f",final_roi[i][0],final_roi[i][1]);
        
    printf("\n\n ROI Done ! Press Esc...");
    printf("\n");
    for(size_t i=0;i<final_roi.size()-1;++i)
        line(camerafeed,Point(final_roi[i][0],final_roi[i][1]),Point(final_roi[i+1][0],final_roi[i+1][1]),Scalar(255,0,0),2,CV_AA);
    line(camerafeed,Point(final_roi[final_roi.size()-1][0],final_roi[final_roi.size()-1][1]),Point(final_roi[0][0],final_roi[0][1]),Scalar(255,0,0),2,CV_AA);

    cvWaitKey(30);
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
