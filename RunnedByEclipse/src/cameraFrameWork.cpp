/*
 * cameraFrameWork.cpp
 *
 *  Created on: Dec 31, 2016
 *      Author: gnouchnam
 */

#include "cameraFrameWork.h"

VideoCapture capture;

// Predefined Functions
void init(int argc, char** args, DataBundle &data){
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
Mat readImage()
#endif
{
	Mat input;
#if STATIC_IMAGE
	string imgPath = DATASET_DIR + imgName;
	cout<<"\nReading Img:"<<imgName<<endl;
	input = imread(imgPath);
#elif VIDEO || CAMERA
	capture >> input;
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
