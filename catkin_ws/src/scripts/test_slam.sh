#!/bin/sh
export ROBOT_INITIAL_POSE="-x 0 -y 0 -z 0"
terminator -e "bash -c 'source ~/RoboticsSoftwareEngineer/project5/catkin_ws/devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch world_file:="$(pwd)/src/map/office"; exec bash'" &
sleep 5
terminator -e "bash -c 'source ~/RoboticsSoftwareEngineer/project5/catkin_ws/devel/setup.bash; roslaunch turtlebot_gazebo gmapping_demo.launch; exec bash'" &
sleep 5
terminator -e "bash -c 'source ~/RoboticsSoftwareEngineer/project5/catkin_ws/devel/setup.bash; roslaunch turtlebot_rviz_launchers view_navigation.launch; exec bash'" &
sleep 5
terminator -e "bash -c 'source ~/RoboticsSoftwareEngineer/project5/catkin_ws/devel/setup.bash; roslaunch turtlebot_teleop keyboard_teleop.launch; exec bash'"
