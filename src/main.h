#ifndef _MAIN_H_
#define _MAIN_H_

#include "common.h"
#include "dirent.h"

// Variables, structures and classes
#if STATIC_IMAGE
#define 	WAIT_TIME			0//2000
	DIR *dir;
	struct dirent *ent;
#elif VIDEO || CAMERA
#define 	WAIT_TIME			1
	VideoCapture capture;
#endif

// Function headers
void init(int argc, char** args);
void exit();


// Predefined Functions
void init(int argc, char** args){

#if STATIC_IMAGE
	dir = opendir (DATASET_DIR);
	assert(dir && "Cannot open image directory");
#elif CAMERA
	int cameraId = 0;
	if(argc > 1)
		cameraId = atoi(args[1]);
	capture = VideoCapture(cameraId);
	assert(capture.isOpened() && "Cannot open the camera");
#elif VIDEO
	capture = VideoCapture(VIDEO_FILE);
	assert(capture.isOpened() && "Cannot open video file");
#endif
}

void exit(){
#if STATIC_IMAGE
	if(dir)
		closedir (dir);
#endif
}

#if STATIC_IMAGE
Mat readImage(string imgName)
#elif VIDEO || CAMERA
Mat readImage(VideoCapture &cap)
#endif
{
	Mat input;
#if STATIC_IMAGE
	string imgPath = DATASET_DIR + imgName;
	cout<<"\nReading Img:"<<imgName<<endl;
	input = imread(imgPath);
#elif VIDEO || CAMERA
	cap >> input;
#endif
	if(input.empty())
		perror("");
	else{
		int newRows = COLS/(input.cols*1.0/input.rows);
		resize(input, input, Size(COLS,newRows));
	}
	return input;
}

#endif // !_MAIN_H_
