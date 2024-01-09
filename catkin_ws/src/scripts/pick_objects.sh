#!/bin/sh
terminator -e "bash -c 'source ~/RoboticsSoftwareEngineer/project5/catkin_ws/devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch; world_file:="$(pwd)/src/map/office"; exec bash'" &
sleep 5
terminator -e "bash -c 'source ~/RoboticsSoftwareEngineer/project5/catkin_ws/devel/setup.bash; roslaunch turtlebot_gazebo amcl_demo.launch; exec bash'" &
sleep 5
terminator -e "bash -c 'source ~/RoboticsSoftwareEngineer/project5/catkin_ws/devel/setup.bash; roslaunch turtlebot_rviz_launchers view_navigation.launch; exec bash'" &
sleep 5
terminator -e "bash -c 'source ~/RoboticsSoftwareEngineer/project5/catkin_ws/devel/setup.bash; rosrun pick_objects pick_objects; exec bash'"