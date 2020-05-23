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
    bool try_use_gpu = false;
    Stitcher::Mode mode = Stitcher::SCANS;
   
    for(int i=1;i<300;i+=25)
    {
    	stringstream ss,ss1;
    	ss << i;
	    ss1 << i+25;
		string path1 = "/home/atul/DeWinter/Stitcher/c19.jpg";
	   	string path2 = "/home/atul/DeWinter/Stitcher/data10/722.jpg";
	   	

	  
		Mat image1 = imread(path1);
		Mat image2 = imread(path2);
		


		vector <Mat> desk;
		desk.push_back(image1);
		desk.push_back(image2);
		
	    Mat result;
		Ptr<Stitcher> stitcher = Stitcher::create(mode, try_use_gpu);
	    Stitcher::Status status = stitcher->stitch(desk, result);
		imwrite("r19.jpg", result);
		desk.clear();
    }
    
	return 0;
}
