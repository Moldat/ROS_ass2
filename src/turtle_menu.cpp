 #include "ros/ros.h"
 #include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>
 #include <sstream>
#include <sensor_msgs/LaserScan.h>

float distanceFromObsticle = 0;
float distanceToMove = 0.5;
float speed = 0.1;
float degree = 0.0174532925;
void moveForward(ros::NodeHandle n);
void turn(ros::NodeHandle n);
void stopRobot(ros::NodeHandle n, geometry_msgs::Twist t);
void scanCallBack(const sensor_msgs::LaserScan::ConstPtr& msg){
	distanceFromObsticle = msg->ranges[0];
	printf("Got distance %f \n", distanceFromObsticle);
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


		}





			  
			  
			
    }

 
 
   return 0;
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