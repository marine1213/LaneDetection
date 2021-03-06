#ifndef _COMMON_H_
#define _COMMON_H_

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/video/background_segm.hpp"

#include <iostream>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */

#include <execinfo.h>
#include <signal.h>

using namespace cv;
using namespace std;

// Type of input (enable for an input only)
#define STATIC_IMAGE				1
#define VIDEO						0
#define CAMERA						0

// Dataset configurations
#define VIDEO_FILE					"../../../Dataset/SangilSamples/test.avi"
#define DATASET_DIR					"Dataset/Source1/"

// Image size
#define COLS						860
#define ROWS						540


#define CHECK_RELATIVE_PT2IPL		0
#define CHECK_LANE_LOCATOR			0
#define CHECK_DISTANCE2STOPLINE		1
#define CHECK_CENTER_PT				0
#define SHOW_INPUT					1
#define	DEBUG						1

#define SHOW_CLASSES_STATE			0
#define SHOW_RF_STATE				0
#define SHOW_PROCESSING_STATE		0

#endif //_COMMON_H_
