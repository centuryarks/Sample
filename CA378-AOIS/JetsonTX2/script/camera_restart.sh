#!/bin/sh
cd /home/nvidia/demo
sudo pkill nvcamera-daemon
sudo nvcamera-daemon &
