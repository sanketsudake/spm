#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
#define LEN 5
float getAvg(float arr[])
{
    float sum = 0;
    for(int i = 0; i < LEN; i++)
	sum += arr[i];
    return (sum / LEN);
}

float getOrderedAvg(float arr[], int index, const float alpha)
{
    float st;
    if(index < LEN)
    {
	st = (arr[index] * (alpha))
	    + ((1 - alpha) * getOrderedAvg(arr, index + 1, alpha));
    }
    else
	return 0;
    return st;
}

int main()
{
    Mat input;
    float arr1[] = { 80, 100, 100, 100, 100};
    float arr2[] = { 100, 100, 100, 100, 80};

    cout << "Actual values" << endl;
    cout << getAvg(arr1) << "\t"
	 << getAvg(arr2) << endl;
    cout << "Alpha\t\tWA1\t\tWA2" << endl;
    for(int i = 0; i < 10; i++)
    {
	float alpha = i * 0.1;
	cout << alpha << "\t\t"
	     << getOrderedAvg(arr1, 0, alpha) << "\t\t"
	     << getOrderedAvg(arr2, 0, alpha) << endl;
    }
    return 0;
}
