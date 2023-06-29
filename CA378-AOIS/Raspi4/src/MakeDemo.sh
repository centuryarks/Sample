#!/bin/sh

# make GUI
cd ./GUI/
qmake
make
mv DemoGUI ../../bin/

