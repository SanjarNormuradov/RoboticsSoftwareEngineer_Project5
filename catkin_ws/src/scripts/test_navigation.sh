#!/bin/sh
terminator -e "bash -c 'source ~/RoboticsSoftwareEngineer/project5/catkin_ws/devel/setup.bash; export ROS_IP=localhost; roslaunch turtlebot_gazebo turtlebot_world.launch world_file:="$(pwd)/src/map/office"; exec bash'" &
sleep 5
terminator -e "bash -c 'source ~/RoboticsSoftwareEngineer/project5/catkin_ws/devel/setup.bash; export ROS_IP=localhost; roslaunch turtlebot_gazebo amcl_demo.launch map_file:="$(pwd)/src/map/office.yaml" initial_pose_a:=-1.57; exec bash'" &
sleep 5
terminator -e "bash -c 'source ~/RoboticsSoftwareEngineer/project5/catkin_ws/devel/setup.bash; export ROS_IP=localhost; roslaunch turtlebot_rviz_launchers view_navigation.launch; exec bash'" &

