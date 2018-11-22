cmake_minimum_required(VERSION 2.8.3)
project(assignment2)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS message_generation roscpp rospy genmsg std_msgs cv_bridge image_transport)
find_package( OpenCV REQUIRED )
include_directories(
  ${catkin_INCLUDE_DIRS}
)

include_directories(include ${catkin_INCLUDE_DIRS})
include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})
## Declare ROS messages and services
# add_message_files(FILES Num.msg)
# add_service_files(FILES AddTwoInts.srv)

## Generate added messages and services

## Declare a catkin package
catkin_package()
 

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(turtle_menu src/turtle_menu.cpp)
target_link_libraries(turtle_menu ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

