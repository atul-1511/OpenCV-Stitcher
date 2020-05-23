#include <iostream>
#include <fstream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"
#include <sstream>
#include <string.h>

using namespace cv;
using namespace std;

int main()
{
	
	int k;
	bool try_use_gpu = false;
    Stitcher::Mode mode = Stitcher::SCANS;
   	for(int i=1, k=1; i<tot_img_row*row_no;i=i+tot_img_row,k++)
    {
    	stringstream ss,ss1;
    	ss << k;
 		ss1 << i;
 		string path1 = "/home/atul/DeWinter/Stitcher/data10/" + ss1.str() + ".jpg" ;
 		Mat image1 = imread(path1);
 		vector <Mat> desk;	
 		desk.push_back(image1);
 		for(int j=i+1;j<i+img_row;j++)
 		{
 			stringstream ss2;
 			ss2 << j;
 			string path2 = "/home/atul/DeWinter/Stitcher/data10/" + ss2.str() + ".jpg" ;
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
				j=j+1;
			}	
			imwrite("r" + ss.str() + ".jpg",image1);
			cout << j << endl;
 		}
 		
    }
    Mat img1 = imread("/home/atul/DeWinter/Stitcher/r1.jpg");
 	vector <Mat> desk1;	
		desk1.push_back(img1);
 	/*for(int i=2;i<=38;i++)
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
 	}*/
    return 0;
}