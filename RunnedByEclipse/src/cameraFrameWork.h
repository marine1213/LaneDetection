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

extern	DIR *dir;
extern	struct dirent *ent;
extern  Mat readImage(string imgName);
#endif
#if VIDEO || CAMERA
#define 	WAIT_TIME			1
	Mat readImage();
#endif

// Function headers
void init(int argc, char** args, DataBundle &data);
void onExit();


#endif /* MAIN_IMGPROCESSINGLIB_H_ */
