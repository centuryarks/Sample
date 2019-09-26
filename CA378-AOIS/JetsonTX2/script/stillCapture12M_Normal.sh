#!/bin/sh
W=4032
H=3040
MODE=0
#W=3840
#H=2160
#MODE=1
#W=1920
#H=1080
#MODE=2
#W=640
#H=480
#MODE=3
ID=$1
EXPOSURE=$2
GAIN=$3
filename=still_${W}x${H}_normal
v4l2-ctl -d /dev/video${ID} --set-fmt-video=width=${W},height=${H},pixelformat=RG10 \
 --set-ctrl sensor_mode=${MODE} \
 --set-ctrl exposure=${EXPOSURE} \
 --set-ctrl gain=${GAIN} \
 --set-ctrl bypass_mode=0 \
 --stream-mmap --stream-skip=1 --stream-count=1 --stream-to=$filename.raw --verbose
./tool/raw2dng -i $filename.raw -o $filename.dng -w ${W} -h ${H} -bit 10 -flip 1 -gain 1.0 1.0 1.0
ufraw $filename.dng
