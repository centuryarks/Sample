#!/bin/sh
cd ./GUI/
qmake
make
mv DemoGUI ../../bin/
cd ../tool/
make
mv raw2dng ../../bin/
