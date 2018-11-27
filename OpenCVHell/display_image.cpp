
/**
 * @file HoughCircle_Demo.cpp
 * @brief Demo code for Hough Transform
 * @author OpenCV team
 */

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

namespace
{
    // windows and trackbars name
    const std::string windowName = "Hough Circle Detection Demo";
    const std::string cannyThresholdTrackbarName = "Canny threshold";
    const std::string accumulatorThresholdTrackbarName = "Accumulator Threshold";
    const std::string usage = "Usage : tutorial_HoughCircle_Demo <path_to_input_image>\n";

    // initial and max values of the parameters of interests.
    const int cannyThresholdInitialValue = 100;
    const int accumulatorThresholdInitialValue = 50;
    const int maxAccumulatorThreshold = 200;
    const int maxCannyThreshold = 255;
    const int BLUE = 0;
    const int GREEN = 1;
    const int RED = 2;

    // bool inMyRange(int test, int upper, int lower){
    //     return ((test <= upper) && (test >= lower));
    // }



    bool isRed(int * pixel){
        return ((pixel[2] > pixel[0]) && (pixel[2] > pixel[1]));

    }

    bool isGreen(int * pixel){
        return ((pixel[1] > pixel[0]) && (pixel[1] > pixel[2]));
    }

    bool isBlue(int * pixel){
        return ((pixel[0] > pixel[1]) && (pixel[0] > pixel[2]));
    }


    int getColor(int * pixel){
        if (isRed(pixel)){
            return RED;
        }
        else if (isGreen(pixel)){
            return GREEN;
        }
        else if (isBlue(pixel)){
            return BLUE;
        }
    }

    void HoughDetection(const Mat& src_gray, const Mat& src_display, int cannyThreshold, int accumulatorThreshold)
    {
        std::cout<<"Searching colors \n";
        // will hold the results of the detection
        std::vector<Vec3f> circles;
        // runs the actual detection
        //HoughCircles( src_gray, circles, HOUGH_GRADIENT, 1, src_gray.rows/8, cannyThreshold, accumulatorThreshold, 0, 0 );

        // clone the colour, input image for displaying purposes
        Mat display = src_display.clone();

 
        int rows = display.rows;
        int cols = display.cols;

        cv::Size s = display.size();
        rows = s.height;
        cols = s.width;

        int output_array[rows*cols] = {0};

        int user_choice = RED;
        int color;
        int k = 0;
        for (int i = 0; i < rows; ++i)
         {
            for (int j = 0; j < cols; ++j)
            {
                int pixel[3] = {
                    display.at<cv::Vec3b>(i,j)[0],
                    display.at<cv::Vec3b>(i,j)[1],
                    display.at<cv::Vec3b>(i,j)[2]
                };
                printf("G %u B %u R %u\n", pixel[0], pixel[1], pixel[2] );
                color = getColor(pixel);
                if (color == user_choice){
                    output_array[k++] = j;
                    // int answer[2] = {i, j};
                    // return &answer;
                }
            }
         } 
         return output_array[k/2];

       

        // shows the results
       // imshow( windowName, display);
    }
}


int main(int argc, char** argv)
{
    Mat src, src_gray;

    // Read the image
    String imageName("Selection_176.png"); // by default
    if (argc > 1)
    {
       imageName = argv[1];
    }
    src = imread( imageName, IMREAD_COLOR );

    if( src.empty() )
    {
        std::cerr<<"Invalid input image\n";
        std::cout<<usage;
        return -1;
    }

    // Convert it to gray
    //cvtColor( src, src_gray, COLOR_BGR2GRAY );

    // Reduce the noise so we avoid false circle detection
   // GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

    //declare and initialize both parameters that are subjects to change
    int cannyThreshold = cannyThresholdInitialValue;
    int accumulatorThreshold = accumulatorThresholdInitialValue;

    // create the main window, and attach the trackbars
    //namedWindow( windowName, WINDOW_AUTOSIZE );
    //createTrackbar(cannyThresholdTrackbarName, windowName, &cannyThreshold,maxCannyThreshold);
    //createTrackbar(accumulatorThresholdTrackbarName, windowName, &accumulatorThreshold, maxAccumulatorThreshold);

    // infinite loop to display
    // and refresh the content of the output image
   HoughDetection(src, cannyThreshold, accumulatorThreshold);
    return 0;
}