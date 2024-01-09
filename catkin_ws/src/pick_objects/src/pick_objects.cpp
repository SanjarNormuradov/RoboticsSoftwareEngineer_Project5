#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/PoseStamped.h>
#include <queue>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

class MultiGoalNavigator {
public:
    MultiGoalNavigator() : ac_("move_base", true), processing_goal_(false)  {
        goal_sub_ = nh_.subscribe("/move_base_simple/goal", 10, &MultiGoalNavigator::goalCallback, this);
        ROS_INFO("Waiting for the move_base action server");
        ac_.waitForServer();
    }

    void goalCallback(const geometry_msgs::PoseStamped::ConstPtr& msg) {
        move_base_msgs::MoveBaseGoal goal;
        goal.target_pose = *msg;
        goals_.push(goal);
        if (!processing_goal_) {
            sendNextGoal();
        }
    }

    void sendNextGoal() {
        if (!goals_.empty()) {
            processing_goal_ = true;
            move_base_msgs::MoveBaseGoal next_goal = goals_.front();
            goals_.pop();
            ROS_INFO("Sending goal");
            ac_.sendGoal(next_goal, boost::bind(&MultiGoalNavigator::goalReachedCallback, this, _1, _2));
        }
    }

    void goalReachedCallback(const actionlib::SimpleClientGoalState& state,
                             const move_base_msgs::MoveBaseResult::ConstPtr& result) {
        if (state == actionlib::SimpleClientGoalState::SUCCEEDED) {
            ROS_INFO("Goal reached!");
        } else {
            ROS_INFO("Failed to reach the goal.");
        }
        processing_goal_ = false;
        sendNextGoal();
    }

private:
    ros::NodeHandle nh_;
    MoveBaseClient ac_;
    ros::Subscriber goal_sub_;
    std::queue<move_base_msgs::MoveBaseGoal> goals_;
    bool processing_goal_;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "pick_objects");
    MultiGoalNavigator navigator;
    ros::spin();
    return 0;
}