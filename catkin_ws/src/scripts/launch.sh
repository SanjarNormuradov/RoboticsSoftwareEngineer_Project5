#!/bin/sh
terminator -e "bash -c 'gazebo; exec bash'" &
sleep 5
terminator -e "bash -c 'roscore; exec bash'" &
sleep 5
terminator -e "bash -c 'rviz; exec bash'"
