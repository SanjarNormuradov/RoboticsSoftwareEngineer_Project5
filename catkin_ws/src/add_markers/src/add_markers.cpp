#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

#include "pick_objects/DriveToGoal.h"

int main( int argc, char** argv )
{
    ros::init(argc, argv, "add_markers");
    ros::NodeHandle nh;
    ros::Publisher marker_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker", 1);
    // Define a client service capable of requesting services from command_robot
    ros::ServiceClient client = nh.serviceClient<pick_objects::DriveToGoal>("/pick_objects/set_goal");

    pick_objects::DriveToGoal srv;
    visualization_msgs::Marker marker;

    for (int i = 0; i < 2; ++i) {
        // Set the frame ID and timestamp.
        marker.header.frame_id = "map";
        marker.header.stamp = ros::Time::now();
        srv.request.goal_pose.header.frame_id = "map";
        srv.request.goal_pose.header.stamp = ros::Time::now();

        // Set the namespace and id for this marker.  This serves to create a unique ID
        // Any marker sent with the same namespace and id will overwrite the old one
        marker.ns = "home_service_robot";
        marker.id = 0;
        // Set the marker type to cube
        marker.type = visualization_msgs::Marker::CUBE;

        // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
        marker.action = visualization_msgs::Marker::ADD;

        if (i == 0) {
            srv.request.goal_type = "pickup";

            // Position and orientation for the robot to reach the pickup zone
            marker.pose.position.x = 0.5;
            marker.pose.position.y = -3.0;
            // marker.pose.position.z = 0;
            // marker.pose.orientation.x = 0.0;
            // marker.pose.orientation.y = 0.0;
            // marker.pose.orientation.z = 0.0;
            marker.pose.orientation.w = 1.0;

            srv.request.goal_pose.pose.position.x = 0.5;
            srv.request.goal_pose.pose.position.y = -3.0;
            // srv.request.goal_pose.pose.position.z = 0;
            // srv.request.goal_pose.pose.orientation.x = 0.0;
            // srv.request.goal_pose.pose.orientation.y = 0.0;
            // srv.request.goal_pose.pose.orientation.z = 0.0;
            srv.request.goal_pose.pose.orientation.w = 1.0;
        } else {
            srv.request.goal_type = "drop-off";

            // Position and orientation for the robot to reach the drop-off zone
            marker.pose.position.x = 4.0;
            marker.pose.position.y = 4.5;
            // marker.pose.position.z = 0;
            // marker.pose.orientation.x = 0.0;
            // marker.pose.orientation.y = 0.0;
            // marker.pose.orientation.z = 0.0;
            marker.pose.orientation.w = 1.0;

            srv.request.goal_pose.pose.position.x = 4.0;
            srv.request.goal_pose.pose.position.y = 4.5;
            // srv.request.goal_pose.pose.position.z = 0;
            // srv.request.goal_pose.pose.orientation.x = 0.0;
            // srv.request.goal_pose.pose.orientation.y = 0.0;
            // srv.request.goal_pose.pose.orientation.z = 0.0;
            srv.request.goal_pose.pose.orientation.w = 1.0;
        }
        // Set the scale of the marker -- 1x1x1 here means 1m on a side
        marker.scale.x = 0.3;
        marker.scale.y = 0.3;
        marker.scale.z = 0.3;
        // Set the color -- be sure to set alpha to something non-zero!
        marker.color.r = 0.0f;
        marker.color.g = 0.0f;
        marker.color.b = 1.0f;
        marker.color.a = 1.0;
        marker.lifetime = ros::Duration();

        // Publish the marker
        while (marker_pub.getNumSubscribers() < 1)
        {
        if (!ros::ok())
        {
            return 0;
        }
        ROS_WARN_ONCE("Please create a subscriber to the marker");
        sleep(1);
        }
        if (srv.request.goal_type == "pickup") {
            ROS_INFO("Visualizing the %s zone (x: %1.2f, y: %1.2f)", srv.request.goal_type.c_str(), srv.request.goal_pose.pose.position.x, srv.request.goal_pose.pose.position.y);
            marker_pub.publish(marker);
        }

        if (client.call(srv)) {
            if (srv.response.success) {
                if (srv.request.goal_type == "pickup") {
                    marker.action = visualization_msgs::Marker::DELETE;
                    marker_pub.publish(marker);
                    ros::Duration(5.0).sleep();
                } else {
                    ROS_INFO("Visualizing the %s zone (x: %1.2f, y: %1.2f)", srv.request.goal_type.c_str(), srv.request.goal_pose.pose.position.x, srv.request.goal_pose.pose.position.y);
                    marker_pub.publish(marker);
                }
            }
        } else {
            ROS_ERROR("Failed to call service set_goal");
        }
        
    }
    ros::spin();

    return 0;
}