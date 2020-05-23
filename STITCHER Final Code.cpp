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
        int rows = img1.rows + img2.rows-150;
        int cols = max(img1.cols,img2.cols)+3;

    // Create a black image
    Mat res(rows, cols, CV_8UC3,Scalar(0,0,0));

    // Copy images in correct position
    // Image 1 is copied to a corner of the Final image
    img1.copyTo(res(Rect(0, 0, img1.cols, img1.rows))); 
    // Image 2 is copied after Image 1 such that it covers the common area
    img2.copyTo(res(Rect(3,img1.rows-150, img2.cols, img2.rows))); 
  
    return res;
}
int main()
{
    bool try_use_gpu = false;
    Stitcher::Mode mode = Stitcher::SCANS;
	
	int no_of_col,img_col,c;
	cout << " Enter the number of images in one column" << endl;
	cin >> img_col;
	cout << " Enter the total number of columns" << endl;
	cin >> no_of_col;
 	
 	for( int i=1; i<= no_of_col ; i=i+1)
 	{
 		stringstream ss1;
 		ss1 << i;
 		string path1 = "/home/atul/DeWinter/Stitcher/" + ss1.str() + ".jpg" ;
 		Mat image1 = imread(path1);
 		vector <Mat> desk;	
 		desk.push_back(image1);
 		for( int j=i+no_of_col ; j<=no_of_col*img_col;j=j+ no_of_col)
 		{
 			
 			stringstream ss2;
 			ss2 << j;
 			string path2 = "/home/atul/DeWinter/Stitcher/" + ss2.str() + ".jpg" ;
 			Mat image2 = imread(path2);
 			desk.push_back(image2);
 			Ptr<Stitcher> stitcher = Stitcher::create(mode, try_use_gpu);
   	   	    Stitcher::Status status = stitcher->stitch(desk, image1);
   	   	    if (status != Stitcher::OK)	
			{
				cout << j;
				cout << "\nUnable to perform stitching...";
				cout << "\nError code: " << status;
				desk.clear();
				j=j+no_of_col;
			}	
			imwrite("r" + ss1.str() + ".jpg",image1);
		}

 	}
 	// Check the length of all the columns
 	float size;
 	size = ((2*0.65*480)+((img_col-2)*0.3*480)+((img_col-1)*0.35*480)-300);
 	
 	for(int i=1;i<=no_of_col;i++)
 	{
 		stringstream ss3;
 		ss3 << i;
 		Mat imgx = imread("/home/atul/DeWinter/Stitcher/c" + ss3.str() + ".jpg");
 		
 		if(imgx.rows < size)
 		{

 			
 			// Merge the elements of that column
 			
 			Mat imga = imread("/home/atul/DeWinter/Stitcher/" + ss3.str() + ".jpg");
 			for(int j= i+no_of_col; j<= no_of_col*img_col; j=j+no_of_col)
 			{
 				stringstream ss4;
 				ss4 << j;
 				Mat imgb = imread("/home/atul/DeWinter/Stitcher/" + ss4.str() +".jpg");
 				imga = Merge(imgb,imga);
 				
 			}
 			imwrite("c" + ss3.str() + ".jpg",imga);
 		}
 		
 	}
 	
 	// Start stitching the columns in pairs
 	Mat img1 = imread("/home/atul/DeWinter/Stitcher/r1.jpg");
 	vector <Mat> desk1;	
		desk1.push_back(img1);
 	for(int i=2;i<=no_of_col;i++)
 	{
 		
		stringstream ss5;
		ss5 << i;
		string path2 = "/home/atul/DeWinter/Stitcher/r" + ss5.str() + ".jpg" ;
		Mat img2 = imread(path2);
		desk1.push_back(img2);
		Ptr<Stitcher> stitcher = Stitcher::create(mode, try_use_gpu);
   	    Stitcher::Status status = stitcher->stitch(desk1, img1);
   	    if (status != Stitcher::OK)	
		{
			cout << i;
			cout << "\nUnable to perform stitching...";
			cout << "\nError code: " << status;
			desk1.clear();
			i= i+1;
		}	
		imwrite("final.jpg",img1);
 	}
	return 0;
}
