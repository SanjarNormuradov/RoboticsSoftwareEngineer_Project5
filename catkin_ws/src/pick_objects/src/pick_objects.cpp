#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
    // Initialize the pick_objects node
    ros::init(argc, argv, "pick_objects");

    //tell the action client that we want to spin a thread by default
    MoveBaseClient ac("move_base", true);

    // Wait 5 sec for move_base action server to come up
    while(!ac.waitForServer(ros::Duration(5.0))){
        ROS_INFO("Waiting for the move_base action server to come up");
    }

    // Define two goals
    move_base_msgs::MoveBaseGoal pickup_goal, drop_off_goal;

    // Set up the frame parameters for the first goal (pickup zone)
    pickup_goal.target_pose.header.frame_id = "map";
    pickup_goal.target_pose.header.stamp = ros::Time::now();
    
    // Define a position and orientation for the robot to reach the pickup zone
    pickup_goal.target_pose.pose.position.x = 0.5;
    pickup_goal.target_pose.pose.position.y = -3.0;
    pickup_goal.target_pose.pose.orientation.w = 1.0;

    // Send the first goal (pickup zone)
    ROS_INFO("Sending robot to the pickup zone");
    ac.sendGoal(pickup_goal);
    ac.waitForResult();
    
    // Check if the robot reached its pickup goal
    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
        ROS_INFO("The robot reached the pickup zone");
        // Wait 5 seconds after reaching the pickup zone
        ros::Duration(5.0).sleep();
    } else {
        ROS_INFO("The robot failed to reach the pickup zone");
    }

    // Set up the frame parameters for the second goal (drop-off zone)
    drop_off_goal.target_pose.header.frame_id = "map";
    drop_off_goal.target_pose.header.stamp = ros::Time::now();
  
    // Define a position and orientation for the robot to reach the drop-off zone
    drop_off_goal.target_pose.pose.position.x = 4.0;
    drop_off_goal.target_pose.pose.position.y = 4.5;
    drop_off_goal.target_pose.pose.orientation.w = 1.0;

    // Send the second goal (drop-off zone)
    ROS_INFO("Sending robot to the drop off zone");
    ac.sendGoal(drop_off_goal);
    ac.waitForResult();
    
    // Check if the robot reached its drop off goal
    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        ROS_INFO("The robot reached the drop off zone");
    else
        ROS_INFO("The robot failed to reach the drop off zone");

    return 0;
}
