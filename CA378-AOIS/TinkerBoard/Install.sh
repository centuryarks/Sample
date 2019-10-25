#!/bin/sh

# make demo.ini
touch demo.ini
echo '#' DEMO Setting>>demo.ini
echo preview=${PWD}/script/preview.sh>>demo.ini
echo stillCapture12M_Normal=${PWD}/script/stillCapture12M_Normal.sh>>demo.ini
echo stillCapture12M_HDR=${PWD}/script/stillCapture12M_HDR.sh>>demo.ini
echo stillCaptureFrame=100>>demo.ini
echo gyroGainRateX=1.00>>demo.ini
echo gyroGainRateY=1.00>>demo.ini
echo autoFocusGain=20.0>>demo.ini
echo autoFocusConfidenceThreshold=10>>demo.ini
echo autoFocusMoveLimit=100>>demo.ini
echo Exposure=2000>>demo.ini
echo Gain=200>>demo.ini
mv demo.ini ./bin/

# make DEMO.desktop
touch DEMO.desktop
echo [Desktop Entry]>>DEMO.desktop
echo Type=Application>>DEMO.desktop
echo Exec=${PWD}/script/"demo.sh">>DEMO.desktop
echo Icon=${PWD}/appicon.png>>DEMO.desktop
echo Name[C]=DEMO>>DEMO.desktop
echo Terminal=false>>DEMO.desktop

# copy files
chmod +x DEMO.desktop
mv DEMO.desktop ~/Desktop/
cd ..


sudo ldconfig
