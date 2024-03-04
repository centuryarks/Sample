#!/bin/sh

# make demo.ini
touch demo.ini
echo '#' DEMO Setting>>demo.ini
echo preview=${PWD}/script/preview.sh>>demo.ini
echo stillCapture=${PWD}/script/stillCapture.sh>>demo.ini
echo tuning-file=${PWD}/tuning-file/imx378-20231108_v2.json>>demo.ini
echo gyroGainRateX=1.00>>demo.ini
echo gyroGainRateY=1.00>>demo.ini
echo autoFocusGain=2.0>>demo.ini
echo autoFocusConfidenceThreshold=10>>demo.ini
echo autoFocusMoveLimit=100>>demo.ini
echo autoFocusFrameSyncMode=0>>demo.ini
echo gain=1.00>>demo.ini
echo shutter=20000>>demo.ini
echo FocusControlMode=0>>demo.ini
echo UseCenter4PointsOfPDdata=0>>demo.ini
echo rotation=0>>demo.ini
mv demo.ini ./bin/

# make demo script
touch demo.sh
echo '#'!/bin/sh>>demo.sh
echo cd ${PWD}>>demo.sh
echo ./bin/DemoGUI -w 4056 -h 3040 -fps 10>>demo.sh
mv demo.sh ./script/

# make preview script
touch preview.sh
echo '#'!/bin/sh>>preview.sh
echo cd ${PWD}>>preview.sh
echo 'libcamera-hello -t 0 --viewfinder-width ${1} --viewfinder-height ${2} --framerate ${3} --tuning-file ${4} ${5}'>>preview.sh
mv preview.sh ./script/

# make Still Capture
touch stillCapture.sh
echo '#'!/bin/sh>>stillCapture.sh
echo cd ${PWD}>>stillCapture.sh
echo 'DATETIME=`date "+%Y%m%d_%H%M%S"`'>>stillCapture.sh
echo 'FILENAME=still_${1}x${2}_${DATETIME}'>>stillCapture.sh
echo 'libcamera-still --width ${1} --height ${2} -o ${FILENAME}.jpg --tuning-file ${3} ${4}'>>stillCapture.sh
echo 'VERSION_CODENAME=$(cat /etc/os-release | grep VERSION_CODENAME | awk -F'=' '{print $2}'>&1)'>>stillCapture.sh
echo 'if [ "${VERSION_CODENAME}" = "bookworm" ]; then'>>stillCapture.sh
echo '    eom ${FILENAME}.jpg &'>>stillCapture.sh
echo 'else'>>stillCapture.sh
echo '    gpicview ${FILENAME}.jpg &'>>stillCapture.sh
echo 'fi'>>stillCapture.sh
mv stillCapture.sh ./script

# copy files
chmod 755 ./script/*.sh
cp ./script/demo.sh ~/Desktop/
cd ..
