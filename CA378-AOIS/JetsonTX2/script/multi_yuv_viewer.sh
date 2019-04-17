#!/bin/bash

FILENAME=/mnt/ram/test
SENSOR_NUMBER=6
WIDTH=640
HEIGHT=480
FPS=30

XPOS=(0 ${WIDTH} `expr ${WIDTH} \* 2` 0 ${WIDTH} `expr ${WIDTH} \* 2`)
YPOS=(0 0 0 ${HEIGHT} ${HEIGHT} ${HEIGHT})

gst-launch-1.0 videomixer name=mix \
 sink_0::xpos=${XPOS[0]} sink_0::ypos=${YPOS[0]} sink_0::alpha=1 sink_0::zorder=0 \
 sink_1::xpos=${XPOS[1]} sink_1::ypos=${YPOS[1]} sink_1::alpha=1 sink_1::zorder=1 \
 sink_2::xpos=${XPOS[2]} sink_2::ypos=${YPOS[2]} sink_2::alpha=1 sink_2::zorder=2 \
 sink_3::xpos=${XPOS[3]} sink_3::ypos=${YPOS[3]} sink_3::alpha=1 sink_3::zorder=3 \
 sink_4::xpos=${XPOS[4]} sink_4::ypos=${YPOS[4]} sink_3::alpha=1 sink_4::zorder=4 \
 sink_5::xpos=${XPOS[5]} sink_5::ypos=${YPOS[5]} sink_3::alpha=1 sink_5::zorder=5 \
 ! videoconvert ! xvimagesink \
 filesrc location=${FILENAME}0.yuv ! videoparse width=${WIDTH} height=${HEIGHT} framerate=${FPS}/1 format=2 ! mix.sink_0 \
 filesrc location=${FILENAME}1.yuv ! videoparse width=${WIDTH} height=${HEIGHT} framerate=${FPS}/1 format=2 ! mix.sink_1 \
 filesrc location=${FILENAME}2.yuv ! videoparse width=${WIDTH} height=${HEIGHT} framerate=${FPS}/1 format=2 ! mix.sink_2 \
 filesrc location=${FILENAME}3.yuv ! videoparse width=${WIDTH} height=${HEIGHT} framerate=${FPS}/1 format=2 ! mix.sink_3 \
 filesrc location=${FILENAME}4.yuv ! videoparse width=${WIDTH} height=${HEIGHT} framerate=${FPS}/1 format=2 ! mix.sink_4 \
 filesrc location=${FILENAME}5.yuv ! videoparse width=${WIDTH} height=${HEIGHT} framerate=${FPS}/1 format=2 ! mix.sink_5
