#!/bin/sh

# make demo.ini
touch demo.ini
echo '#' DEMO Setting>>demo.ini
echo preview=${PWD}/script/preview.sh>>demo.ini
echo stillCapture12M_Normal=${PWD}/script/stillCapture12M_Normal.sh>>demo.ini
echo stillCapture12M_HDR=${PWD}/script/stillCapture12M_HDR.sh>>demo.ini
echo stillCaptureFrame=50>>demo.ini
echo gyroGainRateX=1.00>>demo.ini
echo gyroGainRateY=1.00>>demo.ini
echo autoFocusGain=2.0>>demo.ini
echo autoFocusConfidenceThreshold=10>>demo.ini
echo autoFocusMoveLimit=100>>demo.ini
mv demo.ini ./bin/

# make demo script
touch demo.sh
echo '#'!/bin/sh>>demo.sh
echo cd ${PWD}>>demo.sh
echo ./bin/DemoGUI>>demo.sh
mv demo.sh ./script/

# make highspeed script
touch highspeed.sh
echo '#'!/bin/sh>>highspeed.sh
echo cd ${PWD}>>highspeed.sh
echo '#'./bin/highspeed ./profile/IMX378_640x480_250fps_10bit_2lane.xml 978 256 700 IMX378>>highspeed.sh
echo ./bin/highspeed ./profile/IMX378_640x200_500fps_10bit_2lane.xml 978 256 340 IMX378>>highspeed.sh
mv highspeed.sh ./script/

# make preview script
touch preview.sh
echo '#'!/bin/sh>>preview.sh
echo cd ${PWD}>>preview.sh
echo ./bin/highspeed ./profile/IMX378_640x480_4x4.xml 896 256 2000 IMX378>>preview.sh
mv preview.sh ./script/

# make Still Capture 12M Normal
touch stillCapture12M_Normal.sh
echo '#'!/bin/sh>>stillCapture12M_Normal.sh
echo cd ${PWD}>>stillCapture12M_Normal.sh
echo ./bin/stillsampleRAW16 ./profile/IMX378_3968x3008.xml IMX378_3968x3008.raw '$'1>>stillCapture12M_Normal.sh
echo ./bin/raw2dng -i IMX378_3968x3008.raw -o IMX378_3968x3008.dng -w 3968 -h 3008 -bit 10 -gain 2.13 1.0 1.93>>stillCapture12M_Normal.sh
echo ufraw IMX378_3968x3008.dng --wb=camera>>stillCapture12M_Normal.sh
mv stillCapture12M_Normal.sh ./script

# make Still Capture 12M HDR
touch stillCapture12M_HDR.sh
echo '#'!/bin/sh>>stillCapture12M_HDR.sh
echo cd ${PWD}>>stillCapture12M_HDR.sh
echo ./bin/stillsampleRAW16 ./profile/IMX378_3968x3008_HDR.xml IMX378_3968x3008_HDR.raw '$'1>>stillCapture12M_HDR.sh
echo ./bin/raw2hdr -i IMX378_3968x3008_HDR.raw -o IMX378_3968x3008_HDR -w 3968 -h 3008>>stillCapture12M_HDR.sh
echo ./bin/raw2dng -i IMX378_3968x3008_HDR_L.raw -o IMX378_3968x3008_HDR_L.dng -w 3968 -h 3008 -bit 10 -gain 2.13 1.0 1.93>>stillCapture12M_HDR.sh
echo ./bin/raw2dng -i IMX378_3968x3008_HDR_S.raw -o IMX378_3968x3008_HDR_S.dng -w 3968 -h 3008 -bit 10 -gain 2.13 1.0 1.93>>stillCapture12M_HDR.sh
echo ufraw-batch IMX378_3968x3008_HDR_L.dng --output=IMX378_3968x3008_HDR_L.ppm --overwrite --wb=camera>>stillCapture12M_HDR.sh
echo ufraw-batch IMX378_3968x3008_HDR_S.dng --output=IMX378_3968x3008_HDR_S.ppm --overwrite --wb=camera>>stillCapture12M_HDR.sh
echo python3 ./script/HDR.py>>stillCapture12M_HDR.sh
echo geeqie IMX378_3968x3008_HDR_fusion.ppm>>stillCapture12M_HDR.sh
mv stillCapture12M_HDR.sh ./script

# copy files
chmod 755 ./script/*.sh
chmod 755 ./script/*.py
cp ./script/demo.sh ~/Desktop/
cp ./script/highspeed.sh ~/Desktop/

# config setting
cd ../
sudo patch --merge /boot/config.txt < ./demo/config.patch

# library setting

LIBSSP=libssp.conf
LIBHEX=libhexavision_ctl.conf

if [ -e ./libssp/1.31/lib ]; then
  touch ${LIBSSP}
  echo ${PWD}/libssp/1.31/lib>>${LIBSSP}
  sudo mv ${LIBSSP} /etc/ld.so.conf.d/
else
  echo "Download \"libssp-1.31.tar.gz\" from the following site."
  echo "https://www.visionproc.org/download.php"
  echo "$ tar zxvf libssp-1.31.tar.gz"
  exit 1
fi

if [ -e ./HexaVisionControl/0.92/lib ]; then
  touch ${LIBHEX}
  echo ${PWD}/HexaVisionControl/0.92/lib>>${LIBHEX}
  sudo mv ${LIBHEX} /etc/ld.so.conf.d/
else
  echo "Download \"HexaVisionControl-0.92.tar.gz\" from the following site."
  echo "https://www.visionproc.org/download.php"
  echo "$ tar zxvf HexaVisionControl-0.92.tar.gz"
  exit 1
fi

sudo ldconfig
