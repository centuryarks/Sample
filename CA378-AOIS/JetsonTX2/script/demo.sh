#!/bin/sh
ID=$1
cd /home/nvidia/demo
sudo ./bin/DemoGUI -id $ID -w 4032 -h 3040 -fps 30
