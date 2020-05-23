// Prototype.cpp : Defines the entry point for the console application.
#include <iostream>
#include <fstream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"
#include <sstream>
#include <string.h>


using namespace cv;
using namespace std;


bool check_row(Mat image, int y, Rect roi) //returns true if black pixels found
{
	int black_count = 0;
	for (int x = roi.x; x < roi.width; x++)
	{
		Scalar intensity = image.at<uchar>(y, x);
		if (intensity.val[0] == 0)
		{
			black_count++;
		}
		if (black_count>0)
		{
			return true;
		}
	}
	return false;
}

bool check_col(Mat image, int x, Rect roi) //returns true if black pixels found
{
	int black_count = 0;
	for (int y = roi.y; y < roi.height; y++)
	{
		Scalar intensity = image.at<uchar>(y, x);
		if (intensity.val[0] == 0)
		{
			black_count++;
		}
		if (black_count>0)
		{
			return true;
		}
	}
	return false;
}

void cropper(Mat &src, Mat &dest)
{
	//Crops all blank/black pixels from image borders
	//Minimum area cropped per call = 2x+2y pixels

	//Preserving backup of original input image
	Mat original = src;

	//Convert input image to gray
	cvtColor(src, src, CV_BGR2GRAY);

	//Initialize ROI
	Rect roi(0, 0, src.cols, src.rows);

	while (1)
	{
		//Check edges for black/blank pixels
		bool left_edge = check_col(src, roi.x, roi);
		bool bottom_edge = check_row(src, roi.y + roi.height - 1, roi);
		bool right_edge = check_col(src, roi.x + roi.width - 1, roi);
		bool top_edge = check_row(src, roi.y, roi);

		if (!(left_edge || bottom_edge || right_edge || top_edge))
		{
			//Shrinking each edge of ROI by 1 pixel width
			//To compensate for black/blank pixels lying on ROI boundary
			roi.x++;
			roi.y++;
			roi.height--;
			roi.width--;

			//Extracting ROI
			dest = original(roi);
			break;
		}

		if (left_edge)
		{
			roi.x++;
			roi.width--;
		}

		if (bottom_edge)
		{
			roi.height--;
		}

		if (right_edge)
		{
			roi.width--;
		}

		if (top_edge)
		{
			roi.y++;
			roi.height--;
		}
	}
}

int main()
{
	Mat image1;
	image1 = imread("final.jpg");
	cropper(image1,image1);
	imwrite("a.jpg",image1);
	waitKey(0);	
	return 0;
}