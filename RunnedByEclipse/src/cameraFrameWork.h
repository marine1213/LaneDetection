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

#include "Model/DataBundle.h"


// Variables, structures and classes
#if STATIC_IMAGE
#define 	WAIT_TIME			0//2000
	DIR *dir;
	struct dirent *ent;
	Mat readImage(string imgName);
#elif VIDEO || CAMERA
#define 	WAIT_TIME			1
	Mat readImage();
#endif

// Function headers
void init(int argc, char** args, DataBundle &data);
void onExit();


#endif /* MAIN_IMGPROCESSINGLIB_H_ */
