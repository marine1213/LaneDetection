/*
 * main_imgProcessingLib.h
 *
 *  Created on: Oct 28, 2016
 *      Author: gnouchnam
 */

#ifndef MAIN_IMGPROCESSINGLIB_H_
#define MAIN_IMGPROCESSINGLIB_H_

#include "common.h"
#include "dirent.h"
#include "Model/DataStructure.h"


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
void onExit();


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

void onExit(){
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
//		int newRows = COLS/(input.cols*1.0/input.rows);
//		resize(input, input, Size(COLS,newRows));
//		resize(input, input, Size(input.cols/4*3,input.rows/4*3));
		GaussianBlur(input, input, cv::Size(5,5), 0.3);
	}
	return input;
}

#endif /* MAIN_IMGPROCESSINGLIB_H_ */
