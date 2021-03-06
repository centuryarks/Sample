#!/bin/sh

# make demo.ini
touch demo.ini
echo '#' DEMO Setting>>demo.ini
echo preview = ${PWD}/script/preview.sh>>demo.ini
echo stillCapture12M_Normal = ${PWD}/script/stillCapture12M_Normal.sh>>demo.ini
echo gyroGainRateX=1.00>>demo.ini
echo gyroGainRateY=1.00>>demo.ini
echo autoFocusGain=2.0>>demo.ini
echo autoFocusConfidenceThreshold=10>>demo.ini
echo autoFocusMoveLimit=100>>demo.ini
echo AutoFocusAverageNum=1>>demo.ini
echo Exposure=1000>>demo.ini
echo Gain=200>>demo.ini
echo HDRratio=16>>demo.ini
mv demo.ini ./bin/

# make demo script
touch demo.sh
echo '#'!/bin/sh>>demo.sh
echo ID='$1'>>demo.sh
echo cd ${PWD}>>demo.sh
echo sudo ./bin/DemoGUI -id '$ID' -w 4032 -h 3040 -fps 30>>demo.sh
mv demo.sh ./script/

# make DEMO.desktop
touch DEMO.desktop
echo [Desktop Entry]>>DEMO.desktop
echo Keywords=Drivers';'>>DEMO.desktop
echo Exec=gnome-terminal --geometry=120x32+0+0 --command ${PWD}/script/"'demo.sh 0'">>DEMO.desktop
echo Icon=${PWD}/appicon.png>>DEMO.desktop
echo Terminal=false>>DEMO.desktop
echo Type=Application>>DEMO.desktop
echo NotShownIn=GNOME';'Unity';'KDE';'>>DEMO.desktop
echo Categories=X-XFCE';'GTK';'Settings';'HardwareSettings>>DEMO.desktop
echo Name=Additional Drivers>>DEMO.desktop
echo Comment=Configure third-party and proprietary drivers>>DEMO.desktop
echo X-Ubuntu-Gettext-Domain=software-properties>>DEMO.desktop
echo Name[en_US]=DEMO>>DEMO.desktop
echo Comment[en_US]=>>DEMO.desktop

touch DEMO6.desktop
echo [Desktop Entry]>>DEMO6.desktop
echo Keywords=Drivers';'>>DEMO6.desktop
echo Exec=gnome-terminal --geometry=120x32+3000+2000 --command ${PWD}/script/"'demo6.sh'">>DEMO6.desktop
echo Icon=${PWD}/appicon.png>>DEMO6.desktop
echo Terminal=false>>DEMO6.desktop
echo Type=Application>>DEMO6.desktop
echo NotShownIn=GNOME';'Unity';'KDE';'>>DEMO6.desktop
echo Categories=X-XFCE';'GTK';'Settings';'HardwareSettings>>DEMO6.desktop
echo Name=Additional Drivers>>DEMO6.desktop
echo Comment=Configure third-party and proprietary drivers>>DEMO6.desktop
echo X-Ubuntu-Gettext-Domain=software-properties>>DEMO6.desktop
echo Name[en_US]=DEMO6>>DEMO6.desktop
echo Comment[en_US]=>>DEMO6.desktop

# copy files
chmod +x ./script/*.sh
chmod +x DEMO.desktop
chmod +x DEMO6.desktop
mv DEMO.desktop ~/Desktop/
mv DEMO6.desktop ~/Desktop/
