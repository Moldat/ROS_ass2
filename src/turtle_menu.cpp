 #include "ros/ros.h"
 #include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>
 #include <sstream>
#include <sensor_msgs/LaserScan.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <iostream>
#include <string>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <algorithm>
#include <iterator>

const int distance_arr_size = 360;



float distanceFromObsticle = 0;
float distanceToMove = 0.5;
float speed = 0.1;
float degree = 0.0174532925;
float fov = 1.3962634;
int fovDeg = 80;
int picture_width = 800;
int userColor;
int theXCord = 0;
float distanceFromObject = 0;
float distancesArray[360];
const int BLUE = 0;
const int GREEN = 1;
const int RED = 2;


void cameraCallBack(const sensor_msgs::ImageConstPtr& msg);
void findDistanceFromObject(ros::NodeHandle n);
void moveForward(ros::NodeHandle n);
void turn(ros::NodeHandle n);
void stopRobot(ros::NodeHandle n, geometry_msgs::Twist t);

void scanCallBack(const sensor_msgs::LaserScan::ConstPtr& msg){
	distanceFromObsticle = msg->ranges[0];
	for (int i = 0; i < distance_arr_size; ++i)
	{
		distancesArray [i] = msg->ranges[i];
	}
	
}
 int main(int argc, char **argv)
 {

   ros::init(argc, argv, "turtle_menu");

   ros::NodeHandle n;
      int userInput;

 	ros::Subscriber sub = n.subscribe("scan", 100, scanCallBack);


int c = 10;

    
       std::cout << "The message was published\n";
    while (userInput != 6){




      std::cout << "1. Move forward\n";
      std::cout << "2. Turn around\n";
      std::cout << "3. Distance to object with color X\n";
      std::cout << "4. Find object with color X\n";
      std::cout << "5. Send stop\n";
      std::cout << "6. Exit \n";
      std::cin >> userInput;


      	switch(userInput) {
   case 1  :
      moveForward(n);
      break; //optional
   case 2  :
      turn(n);
      break; //optional

    case 3 : 
    findDistanceFromObject(n);
    break;
		}
			
    }

 
 
   return 0;
 }

 void findDistanceFromObject(ros::NodeHandle n){
 		std::string colorName;


 	    printf("Enter color name\n");
  			std::cin >> colorName;
      	printf("before if\n");
      	if (colorName.compare("green") == 0){
      		userColor = GREEN;
      	} else
      	if (colorName.compare("blue") ==0){
      		userColor = BLUE;
      	}else
      	if (colorName.compare("red") ==0){
      		userColor = RED;
      	}
     	printf("after if the userColor %d\n",userColor);
      	ros::Subscriber sub = n.subscribe("/camera/image_raw", 100, cameraCallBack);
			ros::Rate r(10); // 10 hz
      	while (theXCord == 0)
			{
			  ros::spinOnce();
			  r.sleep();
			}


			printf("The X cord is %d\n", theXCord);




		float m = -(float) fovDeg / (float)picture_width ;
		float c =  (float)fovDeg / 2;

		float fIndex = m*theXCord + c;
				printf("the m is %f, the x is %d, the c is %f\n", m, theXCord, c);
		printf("the fIndex %f\n", fIndex);
			int nIndex = 0;
		if (fIndex > 0)	{
		 
			nIndex = static_cast<int>(fIndex);
		}else{
			nIndex = distance_arr_size + static_cast<int>(fIndex);
		}
		printf("the index is %d \n", nIndex );
		printf("The distance fron %s object is %f \n", colorName.c_str(), distancesArray[nIndex]);
       theXCord = 0;


      	// get the picture
      	// get the x from the picture
      	// calculte the angle
      	// get the distance from the array
 }
bool isRed(int * pixel){
        return ((pixel[2] > 0) && (pixel[1]==0)
            && ( pixel[0] == 0)
            
            );

    }

    bool isGreen(int * pixel){
         return ((pixel[1] > 0) && (pixel[0]==0)
            && ( pixel[2] == 0)
            
            );
     
    }

    bool isBlue(int * pixel){
         return ((pixel[0] > 0) && (pixel[1]==0)
            && ( pixel[2] == 0)
            
            );
       
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
        return -1;
    }
void cameraCallBack(const sensor_msgs::ImageConstPtr& msg){
	printf("Got image\n");
	cv::Mat display = cv_bridge::toCvShare(msg, "bgr8")->image;

	 int rows = display.rows;
        int cols = display.cols;

        cv::Size s = display.size();
        rows = s.height;
        cols = s.width;

        int output_array[rows*cols] = {0};

        int user_choice = userColor;
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
                 
                color = getColor(pixel);
       
                if (color == user_choice){
                    output_array[k++] = j;
                    printf("G %u B %u R %u\n", pixel[0], pixel[1], pixel[2] );
               		printf("adding cord color %d %d \n", i,j);
                    // int answer[2] = {i, j};
                    // return &answer;
                }
            }
         } 
         printf("found X %d, the k is %d\n", output_array[k/2], k);
          theXCord = output_array[k/2];




          if (theXCord == 0){theXCord = -1; }





}
void stopRobot(ros::NodeHandle n, geometry_msgs::Twist t){
				int c=10;
				 ros::Rate r(10);
				 ;
            t.linear.x = speed;
            ros::Publisher movement_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);

    	while (c)
			{
				if (t.linear.x >=0.02){
 
				t.linear.x -=0.02;
				}

       t.angular.z =0;
 
 
			movement_pub.publish(t);
			ros::spinOnce();
			c--;
			r.sleep();
			}
		 
			c=10;
}
 void moveForward(ros::NodeHandle n){
  // int c = 10;
  // ros::Rate r(10); // 10 hz
  //     while (c)
  //     {
  //       ros::spinOnce();
  //       r.sleep();
  //       c--;
        
  //     }
  //     c=10;
  ros::spinOnce();

 		if (distanceFromObsticle > distanceToMove){
 			 
 			geometry_msgs::Twist t;
            t.linear.x = speed;
            t.angular.z = 0;
            ros::Publisher movement_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);

            ros::Rate r(10);
			ros::Time start = ros::Time::now();
			while(ros::Time::now() - start < ros::Duration(5.0))
			{
				printf("Sending move message\n");
			    movement_pub.publish(t);

			    ros::spinOnce();
			    r.sleep();
			}
		 
			// //stop

				stopRobot(n, t);
 			//move
 		}else{
 			printf("Cant move, the distance from obsticle is %f \n", distanceFromObsticle );
 		}
 }

 void turn(ros::NodeHandle n){
  float turnSpeed = 0.2;
  float degrees =0;
      printf("Enter the angle (deg)\n");
      std::cin >>degrees;

      float radians = degrees * degree;

      float dur = radians / turnSpeed;
      geometry_msgs::Twist t;
    
        t.angular.z = -turnSpeed;
        ros::Publisher movement_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);

        ros::Rate r(10);
      ros::Time start = ros::Time::now();
      while(ros::Time::now() - start < ros::Duration(dur))
      {
        printf("Sending move message\n");
          movement_pub.publish(t);

          ros::spinOnce();
          r.sleep();
      }
     
      // //stop

        stopRobot(n, t);
      //move

 }