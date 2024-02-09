#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

#include "pick_objects/DriveToGoal.h"
 
// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

class GoalServiceServer {
public:
    GoalServiceServer() : ac_("move_base", true) {
        // Wait for the action server to come up
        while(!ac_.waitForServer(ros::Duration(5.0))){
            ROS_INFO("Waiting for the move_base action server to come up");
        }

        // Advertise the service
        service_ = nh_.advertiseService("/pick_objects/set_goal", &GoalServiceServer::handle_goal_request, this);
        ROS_INFO("Service server has been started. Ready to reach the goal");
    }

    bool handle_goal_request(pick_objects::DriveToGoal::Request &req,
                             pick_objects::DriveToGoal::Response &res) {
        move_base_msgs::MoveBaseGoal goal;

        // Set up the frame parameters and goal pose
        goal.target_pose = req.goal_pose;
        ROS_INFO("Sending robot to the %s zone (x: %1.2f, y: %1.2f)", req.goal_type.c_str(), goal.target_pose.pose.position.x, goal.target_pose.pose.position.y);

        // Send the goal to the move_base
        ac_.sendGoal(goal);
        // Wait for the result
        ac_.waitForResult();

        if(ac_.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
            ROS_INFO("The robot reached the %s zone", req.goal_type.c_str());
            res.success = true;
        } else {
            ROS_INFO("The robot failed to reach the %s zone", req.goal_type.c_str());
            res.success = false;
        }

        return true;
    }

private:
    ros::NodeHandle nh_;
    MoveBaseClient ac_;
    ros::ServiceServer service_;
};

int main(int argc, char** argv){
    // Initialize the pick_objects node
    ros::init(argc, argv, "pick_objects");
    GoalServiceServer server;

    // Handle ROS communication events
    ros::spin();

    return 0;
}