#include <iostream>
#include <fstream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"
#include <string.h>

using namespace std;
using namespace cv;

int main()
{
	Mat bigImage = imread("image.jpg", CV_LOAD_IMAGE_COLOR);
	cv::Size smallSize(1000,1000);
	std::vector<Mat> smallImages;
	bool x_lim = false, y_lim = false;
	int count = 1;
	for (int i=0;!x_lim;i+=500)
	{
		y_lim = false;
		if(i+smallSize.width<bigImage.cols)
		{	
			for(int j=0;!y_lim;j+=500)
			{
				if(j+smallSize.height<bigImage.rows)
				{	cv::Rect rect =  cv::Rect(i ,j , smallSize.width, smallSize.height);
					smallImages.push_back(cv::Mat(bigImage, rect));
					string path = "/home/atul/DeWinter/Stitcher/data11/" + to_string(count) + ".jpg";
					imwrite(path,smallImages[count-1]);
					count++;
				}
				else{
					y_lim = true;
				}
			}
		}
		else{
			x_lim = true;
		}
	}
}