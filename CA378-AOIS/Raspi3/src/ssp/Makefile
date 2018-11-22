CC = gcc

SSP_LIB = -L/home/pi/libssp/1.31/lib
SSP_INC = -I/home/pi/libssp/1.31/include

MMAL_LIB = -L/opt/vc/lib -lmmal -lmmal_core -lmmal_util -lvcos -lbcm_host -lwiringPi -lpthread

# for Compute Module
#OPENCV_LIB = -L/usr/lib/arm-linux-gnueabihf -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_video -lopencv_core
# for Rpi2
OPENCV_LIB = -L/usr/lib/arm-linux-gnueabihf -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_video -lopencv_core

all: highspeed stillsampleRAW16

highspeed: highspeed.c
	$(CC) -O3 -Wall -o highspeed highspeed.c $(SSP_INC) $(SSP_LIB) $(MMAL_LIB) -lssp -lm $(OPENCV_LIB) -lsspprof -lrt

stillsampleRAW16: stillsampleRAW16.c $(LIBTIFF_LIB)
	$(CC) -o stillsampleRAW16 stillsampleRAW16.c $(SSP_INC) $(SSP_LIB) $(MMAL_LIB) -lssp -lm -lsspprof

clean:
	rm highspeed stillsampleRAW16
