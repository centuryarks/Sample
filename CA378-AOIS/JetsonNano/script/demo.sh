#!/bin/sh
ID=$1
cd /home/nvidia/demo
sudo ./bin/DemoGUI -id $ID -w 4032 -h 3040 -fps 20
#sudo ./bin/DemoGUI -id $ID -w 3840 -h 2160 -fps 30
#sudo ./bin/DemoGUI -id $ID -w 1920 -h 1080 -fps 120
#sudo ./bin/DemoGUI -id $ID -w 640 -h 480 -fps 340
