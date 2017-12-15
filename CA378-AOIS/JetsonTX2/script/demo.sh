#!/bin/sh
cd /home/nvidia/demo
ID=$1
sudo ./bin/DemoGUI -id $1 -w 4056 -h 3040 -fps 30
