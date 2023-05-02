#include <stdio.h>
#include "opencv_aee.hpp"
#include "main.hpp"     // You can use this file for declaring defined values and functions
#include "pi2c.h"

Pi2c car(0x22); // Configure the I2C interface to the Car as a global variabl
float kp=0.3;
float ki=0;
float kd=0;
int sumerror;
using namespace std;
using namespace cv;
int colour = 1; //black=1,blue=2,3=red,4=yellow,5=green,6=everycolour
float u;
void setup(void)
{
    //setupCamera(320, 240);// Enable the camera for OpenCV
    setupCamera(320, 120);
}
  Point findContourCent(std::vector<cv::Point> contour) {

    Moments foundRegion;    // Variables to store the region moment and the centre point
    Point centre;

    foundRegion = moments(contour, false);      // Calculate the moment for the contour

    if (foundRegion.m00 != 0) {
        centre.x = static_cast<int>(foundRegion.m10 / foundRegion.m00);  //Calculate the X and Y positions
        centre.y = static_cast<int>(foundRegion.m01 / foundRegion.m00);
    }
    else {
        // Contour has no area, set center to (0, 0) as a default
        centre = cv::Point(0, 0);
    }

    return centre;
}
int main( int argc, char** argv )
{
    setup();    // Call a setup function to prepare IO and devices

    namedWindow("Photo");   // Create a GUI window called photo

    while(1)    // Main loop to perform image processing
    {
        Mat frame;

        while(frame.empty())
            frame = captureFrame(); // Capture a frame from the camera and store in a new matrix variable

            Mat hsvImg;
          cvtColor(frame, hsvImg, COLOR_BGR2HSV);
          Mat mask;
              if (colour==1){
                Scalar lower(38,0,0);
                Scalar upper(91,106,62);
                inRange(hsvImg, lower, upper, mask);}
                else if (colour==2){
                Scalar lower1(95, 0, 0);
                Scalar upper1(109, 255, 255);
                inRange(hsvImg, lower1, upper1, mask);}
                else if (colour==3){
                Scalar lower2(160, 0, 0);
                Scalar upper2(179, 255, 255);
                inRange(hsvImg, lower2, upper2, mask);}
                else if (colour==4){
                Scalar lower3(4, 100, 10);
                Scalar upper3(34, 230, 230);
                inRange(hsvImg, lower3, upper3, mask);}
                else if (colour==5){
                Scalar lower4(74, 92, 0);
                Scalar upper4(85, 250, 250);
                inRange(hsvImg, lower4, upper4, mask);}
                else if (colour==6){
                Scalar lower5(0, 33,0);
                Scalar upper5(179, 255, 255);
                inRange(hsvImg, lower5, upper5, mask);}
                else {return 0;}
                        // Detect edges using Canny algorithm
                                // Preprocessing - Convert to grayscale and blur image
       
        // Find contours
        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

        // Find the largest contour
        int largestContourIndex = -1;
        double largestContourArea = 0;

        for (size_t i = 0; i < contours.size(); i++)
        {
            double area = contourArea(contours[i]);
            if (area > largestContourArea)
            {
                largestContourIndex = i;
                largestContourArea = area;
            }
        }

        // Draw the largest contour
        if (largestContourIndex >= 0)
        {
            drawContours(frame, contours, largestContourIndex, Scalar(0, 255, 0), 2);
            Point center = findContourCent(contours[largestContourIndex]);
            circle(frame, center,5, Scalar(255,0,0), -1);
             stringstream ss;
          ss << "Center: (" << center.x << ", " << center.y << ")";
           putText(frame, ss.str(), Point(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
           float error=183 - center.x ;
           sleep(0.01);
           float olderror= 183 - center.x;
           sumerror += error;
           u=kp*error+(ki*sumerror)+kd*(error+olderror);
           printf("u = %f \n", u);
        }
        Pi2c arduino(7); //Create a new object "arduino" using address "0x07"
      char receive[16]; //Create a buffer of char (single bytes) for the data

    //Receive from the Arduino and put the contents into the "receive" char array
    arduino.i2cRead(receive,16);
     //Print out what the Arduino is sending...
    std::cout << "Arduino Says: " << receive << std::endl;

    //Send an 16 bit integer
    arduino.i2cWriteArduinoInt(u);
         // Show the resulting image
        imshow("Camera", frame);
        if (waitKey(1) == 27) // Press 'Esc' to exit
        {
            break;
        }

        // Apply color thresholding to HSV image
imshow("Photo", mask);

        int bluePixels=0;

       bluePixels = countNonZero (mask) ;
      std::cout <<"Blue Pixels = " << bluePixels << std::endl;



        int key = cv::waitKey(1);   // Wait 1ms for a keypress (required to update windows)

        key = (key==255) ? -1 : key;    // Check if the ESC key has been pressed
        if (key == 27)
            break;



	}
	closeCV();  // Disable the camera and close any windows

	return 0;
}
