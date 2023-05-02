#include <stdio.h>

#include "opencv_aee.hpp"
#include "main.hpp"     // You can use this file for declaring defined values and functions
#include "pi2c.h"

Pi2c car(0x22); // Configure the I2C interface to the Car as a global variable
using namespace std;
using namespace cv;
void setup(void)
{
    setupCamera(320, 240);  // Enable the camera for OpenCV
}

int main( int argc, char** argv )
{
    setup();    // Call a setup function to prepare IO and devices
    string un = "/home/pi/Pictures/um(Yellow line).bmp";
    string circle = "/home/pi/Pictures/Circle(Red Line).bmp";
    string star = "/home/pi/Pictures/Star(Green Line).bmp";
    string triangle = "/home/pi/Pictures/Triangle(Blue Line).bmp";
    Mat bgrImg1 = imread(un);
    if (bgrImg1.empty())
    {
        cout << "Could not open or find the image 1" << endl;
        return -1;
    }
    Mat bgrImg2 = imread(circle);
    if (bgrImg2.empty())
    {
        cout << "Could not open or find the image 2" << endl;
        return -1;
    }
    Mat bgrImg3 = imread(star);
    if (bgrImg3.empty())
    {
        cout << "Could not open or find the image 3" << endl;
        return -1;
    }
    Mat bgrImg4 = imread(triangle);
    if (bgrImg4.empty())
    {
        cout << "Could not open or find the image 4" << endl;
        return -1;
    }
           Mat hsvImg1;
           cvtColor(bgrImg1, hsvImg1, COLOR_BGR2HSV);
           Mat hsvImg2;
           cvtColor(bgrImg2, hsvImg2, COLOR_BGR2HSV);
           Mat hsvImg3;
           cvtColor(bgrImg3, hsvImg3, COLOR_BGR2HSV);
           Mat hsvImg4;
           cvtColor(bgrImg4, hsvImg4, COLOR_BGR2HSV);
        // Get current trackbar values
          Scalar lower(145, 50, 50);
          Scalar upper(175, 255, 255);

        // Apply color thresholding to HSV image
          Mat mask1;
          inRange(hsvImg1, lower, upper, mask1);
          Mat mask2;
          inRange(hsvImg2, lower, upper, mask2);
          Mat mask3;
          inRange(hsvImg3, lower, upper, mask3);
          Mat mask4;
          inRange(hsvImg4, lower, upper, mask4);
          resize(mask1, mask1, Size(320, 240));
          resize(mask2, mask2, Size(320, 240));
          resize(mask3, mask3, Size(320, 240));
          resize(mask4, mask4, Size(320, 240));
         imshow("HSV", mask1);
         imshow("HSV1", mask2);
         imshow("HSV2", mask3);
         imshow("HSV3", mask4);
         namedWindow("Photo");
    cv::namedWindow("Photo");   // Create a GUI window called photo

    while(1)    // Main loop to perform image processing
    {
        Mat frame;

        while(frame.empty())
            frame = captureFrame(); // Capture a frame from the camera and store in a new matrix variable
          Mat hsvImg;
        cvtColor(frame, hsvImg, COLOR_BGR2HSV);

        // Get current trackbar values
        Scalar lower(145, 50, 50);
        Scalar upper(175, 255, 255);

        // Apply color thresholding to HSV image
        Mat shapes;
        inRange(hsvImg, lower, upper, shapes);

        // Find contours
         vector<vector<Point>> contours;
         vector<Vec4i> hierarchy;
         findContours(shapes, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

         if (!contours.empty()) {
        int largestContourIndex = -1;
        double largestContourArea = 0;
        vector<vector<Point>> approxContours(contours.size());

        for (size_t i = 0; i < contours.size(); i++) {
        approxPolyDP(contours[i], approxContours[i], 30, true);
        double area = contourArea(approxContours[i]);
        if (area > largestContourArea) {
            largestContourArea = area;
            largestContourIndex = i;
          }
         }

        if (largestContourIndex != -1) {
        drawContours(frame, approxContours, largestContourIndex, Scalar(0, 255, 0), 2);

        Mat comparison;
        Mat new_image = transformPerspective(approxContours[largestContourIndex], shapes, 320, 240);
        if ((new_image.cols > 0) && (new_image.rows > 0)) {
            imshow("transformed image2", new_image);
            // Compare new_image with each of the masks
        Mat cmp1, cmp2, cmp3, cmp4;
        compare(new_image, mask1, cmp1, CMP_EQ);
        compare(new_image, mask2, cmp2, CMP_EQ);
        compare(new_image, mask3, cmp3, CMP_EQ);
        compare(new_image, mask4, cmp4, CMP_EQ);

        // Count the number of matching pixels in each comparison matrix
        double totalPixels1 = cmp1.total();
        double totalPixels2 = cmp2.total();
        double totalPixels3 = cmp3.total();
        double totalPixels4 = cmp4.total();
        double matches1 = countNonZero(cmp1);
        double matches2 = countNonZero(cmp2);
        double matches3 = countNonZero(cmp3);
        double matches4 = countNonZero(cmp4);

        // Determine if any of the masks match the image
        bool match1 = (matches1 >= 0.8 * totalPixels1);
        bool match2 = (matches2 >= 0.85 * totalPixels2);
        bool match3 = (matches3 >= 0.85 * totalPixels3);
        bool match4 = (matches4 >= 0.85 * totalPixels4);

        // Print the results
       if (match1) {
        cout << "Image matches un" << endl;
        }
       if (match2) {
        cout << "Image matches circle" << endl;
        }
       if (match3) {
         cout << "Image matches star" << endl;
        }
       if (match4) {
       cout << "Image matches triangle" << endl;
            }

          }
         }
        }


        cv::imshow("Photo", frame); //Display the image in the window

        int key = cv::waitKey(100);   // Wait 1ms for a keypress (required to update windows)

        key = (key==255) ? -1 : key;    // Check if the ESC key has been pressed
        if (key == 27)
            break;
	}
     int key = cv::waitKey(5);
	closeCV();  // Disable the camera and close any windows

	return 0;
} //symbol  code
