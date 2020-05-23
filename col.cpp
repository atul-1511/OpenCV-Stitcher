#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"
#include <sstream>
#include <string.h>

using namespace std;
using namespace cv;

int main()
{
	bool try_use_gpu = false;
    Stitcher::Mode mode = Stitcher::SCANS;
	int no_of_col;
	cout << " enter the number of columns" << endl;
	cin >> no_of_col;
	int x = no_of_col % 10;
	cout <<  x << endl;

	for(int i=1;i<=no_of_col-x;i=i+10)
	{
		stringstream ss1,ss2,ss3,ss4;
		ss1 << i;
		ss2 << i+1;
		
		Mat image1 = imread("/home/atul/DeWinter/Stitcher/c" + ss1.str() + ".jpg");
		Mat image2 = imread("/home/atul/DeWinter/Stitcher/c" + ss2.str() + ".jpg");
		
		vector <Mat> desk1;	
		desk1.push_back(image1);
		desk1.push_back(image2);
		Mat part;
		Ptr<Stitcher> stitcher = Stitcher::create(mode, try_use_gpu);
   	    Stitcher::Status status = stitcher->stitch(desk1, part);
   	    if (status != Stitcher::OK)	
		{
			cout << i;
			cout << "\nUnable to perform stitching...";
			cout << "\nError code: " << status;
			desk1.clear();
			i= i+1;
		}	
		imwrite("part" + ss1.str() + ".jpg",part);
		desk1.clear();
	}
	int i = no_of_col-x;
	stringstream ss;
	ss << i;
	Mat image1 = imread("/home/atul/DeWinter/Stitcher/c" + ss.str() + ".jpg");
	vector <Mat> desk1;	
	desk1.push_back(image1);
	for(int i=no_of_col; i>no_of_col-x ;i--)
	{
		stringstream ss5;
		ss5 << i;
		Mat image2 = imread("/home/atul/DeWinter/Stitcher/c" + ss5.str() + ".jpg");
		desk1.push_back(image2);
		Mat part;
		Ptr<Stitcher> stitcher = Stitcher::create(mode, try_use_gpu);
   	    Stitcher::Status status = stitcher->stitch(desk1, image1);
   	    if (status != Stitcher::OK)	
		{
			cout << i;
			cout << "\nUnable to perform stitching...";
			cout << "\nError code: " << status;
			desk1.clear();
			i= i+1;
		}	
		imwrite("part" + ss5.str() + ".jpg",image1);
		desk1.clear();
	}
	return 0;
}