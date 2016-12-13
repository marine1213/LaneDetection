#ifndef _COMMON_H_
#define _COMMON_H_

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/video/background_segm.hpp"

#include <iostream>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */

using namespace cv;
using namespace std;

// Type of input (enable for an input only)
#define STATIC_IMAGE				0
#define VIDEO						0
#define CAMERA						1

// Dataset configurations
#define VIDEO_FILE					"../../../Dataset/SangilSamples/test.avi"
#define DATASET_DIR					"../../../Dataset/RoadSamples/"

// Image size
#define COLS						860
#define ROWS						540

#define SHOW_INPUT					1
#define	DEBUG						1

#endif //_COMMON_H_
