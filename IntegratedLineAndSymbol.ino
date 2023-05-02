// Include files for required libraries
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
float u;
int match=0;
void setup(void)
{
    //setupCamera(320, 240);// Enable the camera for OpenCV
    setupCamera(320, 240);

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
        setup();    // Call a setup function to prepare IO and devices
    string un = "/home/pi/Pictures/um(Yellow line).bmp";
    string circle1 = "/home/pi/Pictures/Circle(Red Line).bmp";
    string star = "/home/pi/Pictures/Star(Green Line).bmp";
    string triangle = "/home/pi/Pictures/Triangle(Blue Line).bmp";
    Mat bgrImg1 = imread(un);
    if (bgrImg1.empty())
    {
        cout << "Could not open or find the image 1" << endl;
        return -1;
    }
    Mat bgrImg2 = imread(circle1);
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
          Scalar lower9(145, 50, 50);
          Scalar upper9(175, 255, 255);

        // Apply color thresholding to HSV image
          Mat mask1;
          inRange(hsvImg1, lower9, upper9, mask1);
          Mat mask2;
          inRange(hsvImg2, lower9, upper9, mask2);
          Mat mask3;
          inRange(hsvImg3, lower9, upper9, mask3);
          Mat mask4;
          inRange(hsvImg4, lower9, upper9, mask4);
          resize(mask1, mask1, Size(320, 240));
          resize(mask2, mask2, Size(320, 240));
          resize(mask3, mask3, Size(320, 240));
          resize(mask4, mask4, Size(320, 240));
    namedWindow("Photo");   // Create a GUI window called photo

    while(1)    // Main loop to perform image processing
    {
        Mat frame;

        while(frame.empty())
            frame = captureFrame(); // Capture a frame from the camera and store in a new matrix variable

        Mat hsvImg10;
        cvtColor(frame, hsvImg10, COLOR_BGR2HSV);

        Scalar lower9(145, 50, 50);
        Scalar upper9(175, 255, 255);
        Mat shapes;
        inRange(hsvImg10, lower9, upper9, shapes);

        // Find contours
         vector<vector<Point>> contours1;
         vector<Vec4i> hierarchy;
         findContours(shapes, contours1, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

         if (!contours1.empty()) {
        int largestContourIndex = -1;
        double largestContourArea = 0;
        vector<vector<Point>> approxContours(contours1.size());

        for (size_t i = 0; i < contours1.size(); i++) {
        approxPolyDP(contours1[i], approxContours[i], 30, true);
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


       if (match1) {
        cout << "Image matches un" << endl;
              match=1;
        }
       if (match2) {
        cout << "Image matches circle" << endl;
                match=2;
        }
       if (match3) {
         cout << "Image matches star" << endl;
               match=3;
        }
       if (match4) {
       cout << "Image matches triangle" << endl;
                match=4;
                }}}}


          Mat hsvImg;
          cvtColor(frame, hsvImg, COLOR_BGR2HSV);
          Mat mask;
        cv::Scalar lower1(95, 0, 0),upper1(109, 255, 255);
        cv::Scalar lower2(170, 80, 100),  upper2(180, 255, 255);
        cv::Scalar lower3(4, 100, 10), upper3(34, 230, 230);
        cv::Scalar lower4(74, 92, 0), upper4(85, 250, 250, 0);
        cv::Scalar lower(38, 0, 0), upper(91, 106, 62);
                     switch (match) {
        case 1:
        inRange(hsvImg, lower3, upper3, mask);
        break;
        case 2:
        inRange(hsvImg, lower2, upper2, mask);
        break;
        case 3:
        inRange(hsvImg, lower4, upper4, mask);
        break;
        case 4:
        inRange(hsvImg, lower1, upper1, mask);
        break;
        default:
        inRange(hsvImg, lower, upper, mask);
        break;
        }

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
        // Check if contour meets criteria for colored line



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
        }else{match=0;}
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
//inter
