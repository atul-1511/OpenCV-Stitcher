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
	//Set GPU flag
	bool try_use_gpu = false;
        Stitcher::Mode mode = Stitcher::SCANS;

	//Set total image count and row length
	int total_images = 2;
	int row_length = 2;

	//Calculate number of rows
	//int no_of_rows = (total_images / row_length);
	int no_of_rows = 1;

	//Create a vector to store input images
	vector <Mat> inputs;

	string input_folder = "/home/atul/DeWinter/Stitcher/data3";
	for (int i = 1; i <= 2; i++)
	{
		string path = input_folder;
		path.append("/");
		stringstream ss;
		ss << i;
		string str = ss.str();
		path.append(str);
		path.append(".jpg");

		Mat temp = imread(path);
		inputs.push_back(temp);
	}

	cout << "Loaded all input images to inputs vector...";

	//Create a vector to store rows
	vector <Mat> rows;

	for (int i = 0; i < no_of_rows*row_length; i = i + row_length)
	{
		vector <Mat> desk; //vector that stores stiched result and next image to be stiched (never more than 2 elements)
		Mat temp_row = inputs[i];
		desk.push_back(temp_row);

		cout << "\n" << i << " ";

		for (int j = i + 1; j < i + row_length; j++)
		{
			cout << j << " ";

			desk.push_back(inputs[j]);
			Ptr<Stitcher> stitcher = Stitcher::create(mode, try_use_gpu);
            Stitcher::Status status = stitcher->stitch(desk, temp_row);

			//Stitcher stitcher = Stitcher::createDefault(false);
			//Stitcher::Status status = stitcher.stitch(desk, temp_row);

			if (status != Stitcher::OK)
			{
				cout << "\nUnable to perform stitching...";
				cout << "\nError code: " << status;
				getchar();
				return 1;
			}

			
			desk.clear();
			desk.push_back(temp_row);
			//imshow("temp_row", temp_row); waitKey(0); destroyWindow("temp_row");
		}
		rows.push_back(temp_row);
	}

	cout << "\nGeneration of rows complete...";

	///*
	//Display and/or write all stitched rows
	for (int a = 0; a < rows.size(); a++)
	{
		//imshow("Individual Rows", rows[a]); waitKey(0);
		string dest_path = "/home/atul/DeWinter/Stitcher/";
		dest_path.append("");
		stringstream ss;
		ss << a;
		string str = ss.str();
		dest_path.append(str);
		dest_path.append(".jpg");
		imwrite(dest_path, rows[a]);
	}
	//*/

	//All rows generated at this point
	//Stitching all rows together

	///*
	vector <Mat> desk;
	Mat temp_result = rows[0];
	desk.push_back(temp_result);

	for (int i = 1; i < no_of_rows; i++)
	{
		desk.push_back(rows[i]);

        Ptr<Stitcher> stitcher = Stitcher::create(mode, try_use_gpu);
		//Stitcher stitcher = Stitcher::create(mode,try_use_gpu);
		Stitcher::Status status = stitcher->stitch(desk, temp_result);
		//Stitcher::Status status = stitcher.stitch(desk, temp_result);
		cropper(temp_result, temp_result);
		desk.clear();
		desk.push_back(temp_result);
		//imshow("temp_result", temp_result); waitKey(0); destroyWindow("temp_result");
	}

	string dest_path = "/home/atul/final.jpg";
	imwrite(dest_path, temp_result);
	//*/
	//getchar();
	return 0;
}
