CC = gcc

#OPENCV_LIB = -L/usr/lib/arm-linux-gnueabihf -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_video
OPENCV_LIB = -L/usr/lib/arm-linux-gnueabihf -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_video -lopencv_videoio

all: raw2dng raw2hdr preview

raw2dng: raw2dng.c
	$(CC) -o raw2dng raw2dng.c -ljpeg -lm -lz -ltiff

raw2hdr: raw2hdr.c
	$(CC) -o raw2hdr raw2hdr.c -ljpeg -lm -lz -ltiff

preview: preview.c
	$(CC) -O3 -Wall -o preview preview.c -lm $(OPENCV_LIB)

clean:
	rm raw2dng raw2hdr preview
