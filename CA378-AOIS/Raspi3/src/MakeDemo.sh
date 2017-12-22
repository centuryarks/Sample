#!/bin/sh

# make GUI
cd ./GUI/
qmake
make
mv DemoGUI ../../bin/

# make tool
cd ../tool/
make
mv raw2dng ../../bin/
mv raw2hdr ../../bin/
cd ../ssp/

# make viewer
if [ -f ~/highspeed/highspeed.c ]; then
  cp ~/highspeed/highspeed.c ./
  patch < highspeed.patch
else
  echo "Download \"highspeed_20161219.zip\" from the following site."
  echo "https://www.visionproc.org/download.php"
  echo "$ unzip highspeed_20161219.zip -d ~/highspeed"
  exit 1
fi

if [ -f ~/libssp/1.28/sample/stillsampleRAW16/stillsampleRAW16.c ]; then
  cp ~/libssp/1.28/sample/stillsampleRAW16/stillsampleRAW16.c ./
  patch < stillsampleRAW16.patch
else
  echo "Download \"libssp-1.28.tar.gz\" from the following site."
  echo "https://www.visionproc.org/download.php"
  echo "$ tar zxvf libssp-1.28.tar.gz"
  exit 1
fi

make
mv highspeed ../../bin/
mv stillsampleRAW16 ../../bin/
