#include <thread> 
#include <iostream>
#include <iomanip> 
#include <chrono>
#include <ctime>
#include <fstream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"
#include <sstream>
#include <string.h>
#include <math.h>

using namespace std;
using namespace cv;
int img_col,tot_img;
void Stitch(int i)
{
   bool try_use_gpu = false;
   Stitcher::Mode mode = Stitcher::SCANS;
   stringstream ss;
   ss << i;
   string path1 = "/home/atul/DeWinter/Stitcher/" + ss.str() + ".jpg" ;
   Mat image1 = imread(path1);
   vector <Mat> desk;   
   desk.push_back(image1);
   for( int j=i+img_col ; j<=tot_img;j=j+img_col)
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
         j=j+img_col;
      }  
      imwrite("c" + ss.str() + ".jpg",image1);
   }
}
void task1() 
{ 
   Stitch(1);
}

void task2() 
{ 
   Stitch(2);
}
int main () 
{
   cout << "enter total number of images" << endl;
   cin >> tot_img;
   cout << "enter the number of images in one column" << endl;
   cin >> img_col;
   bool try_use_gpu = false;
   Stitcher::Mode mode = Stitcher::SCANS;
   thread thread_1 = thread(task1);
   thread thread_2 = thread(task2);
   thread_2.join();
   thread_1.join();

   Mat img1 = imread("c1.jpg");
   vector <Mat> desk1;  
      desk1.push_back(img1);
   for(int k=2;k<=2;k++)
   {
      
      stringstream ss5;
      ss5 << k;
      string path2 = "c" + ss5.str() + ".jpg" ;
      Mat img2 = imread(path2);
      desk1.push_back(img2);
      Ptr<Stitcher> stitcher = Stitcher::create(mode, try_use_gpu);
          Stitcher::Status status = stitcher->stitch(desk1, img1);
          if (status != Stitcher::OK)  
      {
         cout << k;
         cout << "\nUnable to perform stitching...";
         cout << "\nError code: " << status;
         desk1.clear();
         k= k+1;
      }  
      imwrite("part1.jpg",img1);
   }
    return 0;
}
