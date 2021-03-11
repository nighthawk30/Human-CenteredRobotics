/*
  Nathan Taylor
  3/10/21
*/
#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"
#include "sensor_msgs/LaserScan.h"
#include "std_msgs/String.h"
#include <string>
#include <map>
#include <cstdlib>

#include <cmath>
ros::Publisher pub;
ros::Subscriber sub;

/*
void move(double distance);
void turn(double angle);//ccw
*/
void moveTurn(double distance, double ang_degrees);//ccw+

void poseCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
  ROS_INFO("Min=: [%f]",scan->ranges[scan->angle_min]);
  ROS_INFO("Half=: [%f]",scan->ranges[scan->angle_max/2]);
  ROS_INFO("Max=: [%f]",scan->ranges[scan->angle_max]);
  ROS_INFO("---------------------------");
}

int main(int argc, char **argv)
{
  // INITIALIZE THE NODE
  ros::init(argc, argv, "wall_flower");
  ros::NodeHandle node;
  // Loop at 10Hz, publishing movement commands until we shut down
  ros::Rate rate(10);
  pub = node.advertise<geometry_msgs::Pose2D>("/triton_lidar/vel_cmd", 10);
  sub = node.subscribe<sensor_msgs::LaserScan>("/scan", 1000, poseCallback);
  //init time
  ros::Duration(2.0).sleep();
  //
  ROS_INFO("Get RRRReeadddy toooooo Ruuummmmmmblllleee");
  //Setup
  moveTurn(5,0);
  moveTurn(0,-90);
  //


  //DEFINE STATE ACTION PAIRS
  std::map<std::string, double> qtable;//state, action(drive = .1, angle)
  qtable["close"] = -5;
  qtable["med"] = 0;
  qtable["far"] = 5;


  double etime = ros::Time::now() + ros::Duration(10.0);
  while (ros::Time::now() < etime)
    {
      //check state

      //choose action
    }

  ros::spin();
}

//need to be able to do both at once
void moveTurn(double distance, double ang_degrees)
{
  double angular_speed = .2;
  if (ang_degrees <= 0)
    angular_speed *= -1;
  double speed = .5;
  double PI = 3.141592653589693;
  double ang_rad = ang_degrees * PI/180;
  double cdist = 0;//current distance
  double cang = 0;//current angle
  geometry_msgs::Pose2D stop;

  double start = ros::Time::now().toSec();
  while (cang < ang_rad || cdist < distance)
    {
      cang = angular_speed * (ros::Time::now().toSec() - start);
      cdist = speed * (ros::Time::now().toSec() - start);
      //set msg
      geometry_msgs::Pose2D msg;
      if (abs(cang) < abs(ang_rad))
	msg.theta = angular_speed;
      if (cdist < distance)
	msg.x = speed;
      //print message
      pub.publish(msg);
      //give up
      ros::spinOnce();
    }
  pub.publish(stop);
}
/*
void turn(double angle)
{
  //ccw is positive
  double angular_speed = 1;
  double PI = 3.141592653589693;
  double radians = angle * PI/180;

  geometry_msgs::Pose2D msg;
  geometry_msgs::Pose2D stop;//stop the rotation
  if (angle >= 0)
    msg.theta = angular_speed;
  else
    msg.theta = -angular_speed;
  
  double current_angle = 0;
  double start = ros::Time::now().toSec();
  while (abs(current_angle) < abs(radians))
    {
      current_angle = angular_speed * (ros::Time::now().toSec() - start);
      pub.publish(msg);
      ros::spinOnce();//gives up control for a little bit so laser can scan
    }
  pub.publish(stop);
}

void move(double distance)
{
  double speed = 1;
  geometry_msgs::Pose2D msg;
  geometry_msgs::Pose2D stop;
  msg.x = speed;

  double current_distance = 0;
  double start = ros::Time::now().toSec();
  while (current_distance < distance)
    {
      current_distance = speed * (ros::Time::now().toSec() - start);
      pub.publish(msg);
      ros::spinOnce();//give up control each cycle for distributed system
    }
  pub.publish(stop);
}
*/
