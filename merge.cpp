#include <iostream>
#include <fstream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"
#include <sstream>
#include <string.h>

using namespace cv;
using namespace std;
Mat Merge(Mat img1, Mat img2)
{
    // Get dimension of final image(
        // In case of 640*480 - the dimension of the final image will be for Row Wise Merging- 
        // 640(first image) + (640 - Overlap Area)(final image)
        int rows = max(img1.rows,img2.rows)+10;
        int cols = (img1.cols+img2.cols)-775;

    // Create a black image
    Mat res(rows, cols, CV_8UC3,Scalar(0,0,0));

    // Copy images in correct position
    // Image 1 is copied to a corner of the Final image
    img1.copyTo(res(Rect(0, 0, img1.cols, img1.rows))); 
    // Image 2 is copied after Image 1 such that it covers the common area
    img2.copyTo(res(Rect(img1.cols-775,10, img2.cols, img2.rows))); 
  
    return res;
}
int main()
{
    // Read the images

	Mat image1 = imread("1.jpg");
    for(int i=2;i<=10;i=i+1)
    {
        stringstream ss1;
        ss1 << i;
        Mat image2 = imread("/home/atul/DeWinter/Stitcher/" + ss1.str() +".jpg");
        image1 = Merge(image1,image2);
        imwrite("c7.jpg", image1);
    }
    
    
    return 0;
}
