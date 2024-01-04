#include "ros/ros.h"
#include "std_msgs/String.h"
#include <assignment_2_2023/PlanningAction.h>
#include "assignment_2_2023/Parameters.h"
#include "assignment_2_2023/LastTarget.h"
#include "assignment_2_2023/AverageSpeedAndDistance.h"
#include <geometry_msgs/Point.h>
/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */

float pos_x,pos_y,vel_x,vel_z;

assignment_2_2023::LastTarget lastTarget;

//last target position x and y
float lastPosition_x;
float lastPosition_y;


int windowSize; //window used for the average
//sum of the [windowSize] velocities 
float sumVel_x=0; 
float sumVel_z=0;
int i=0; //counter used for the average
float average_vel_x;
float average_vel_z;
//distance from target to current position
float dist_x;
float dist_y;

//callBack function for the subscribe that get the positions and the velocities 
void parameterCallback(const assignment_2_2023::Parameters::ConstPtr& msg)
{
pos_x= msg->pos_x;
pos_y= msg->pos_y;
vel_x= msg->vel_x;
vel_z= msg->vel_z;

/*ROS_INFO("I heard pos x: [%f], pos y: [%f] , vel x: [%f] ,vel z: [%f] ",
  	 pos_x,
  	 pos_y,
  	 vel_x,
  	 vel_z);*/
  	 
  	 //put in the variables the difference between the targets and the actual positions
  	 dist_x=pos_x-lastPosition_x; 
  	 dist_y=pos_y-lastPosition_y;
 // ROS_INFO("distance from target x: %f y:%f", (pos_x-lastPosition_x),(pos_y-lastPosition_y));
 	 
}


//this function set the results with the distance between target and position and the speed's average
bool VelocityAndDistance(assignment_2_2023::AverageSpeedAndDistance::Request &req, 				assignment_2_2023::AverageSpeedAndDistance::Response &res)
{
	//actionlib::SimpleActionClient<assignment_2_2023::PlanningAction> ac("/reaching_goal", true);
	
	res.dist_x=dist_x;
	res.dist_y=dist_y;
	res.average_x=average_vel_x;
	res.average_z=average_vel_z;

	return true;
}


int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<assignment_2_2023::LastTarget>("/lastTarget");
  ros::ServiceServer service= n.advertiseService("/VelocityDistance", VelocityAndDistance);
  // rosservice call /VelocityDistance 
  //copy and paste this command line on terminal to see the results with the distance between target and position and the speed's average
	
	
	
	client.waitForExistence();
	client.call(lastTarget);
	lastPosition_x= lastTarget.response.target_pose.pose.position.x;
	lastPosition_y= lastTarget.response.target_pose.pose.position.y;
	ros::param::get("/des_window", windowSize);


 
  	ros::Subscriber sub = n.subscribe("robot/parameters", 1, parameterCallback);
  	
	while(ros::ok())
	{
		//calculate the average
	 	sumVel_x+=vel_x; //accumulator of the velocities
 	 	sumVel_z+=vel_z; 
  	 	i++;
	  	 if(i==windowSize) //when i==windowsize  it divide the sumVel for the windowSize and calculate the average
	  	 {
	  	 	average_vel_x=sumVel_x/windowSize;
	  	 	average_vel_z=sumVel_z/windowSize;
	  	 	//ROS_INFO("\n \n average vel x %f \n aveage vel y %f \n \n",average_vel_x,average_vel_z);
	  	 	
	  	 	
	  	 	sumVel_x=0;
	  	 	sumVel_z=0;
	  	 	i=0;
	  	 }
	  	 
		ros::spinOnce();

	}
  



 
 // ros::spin();
  


  return 0;
}
